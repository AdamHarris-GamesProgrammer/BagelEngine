#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Bagel {


    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    public:
        Instrumentor()
            : _currentSession(nullptr), _profileCount(0)
        {
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            _outputStream.open(filepath);
            if (_outputStream.bad()) {
                BG_CORE_ASSERT(false, "Failed to open profiler file")
            }

            WriteHeader();
            _currentSession = new InstrumentationSession{ name };
        }

        void EndSession()
        {
            WriteFooter();
            _outputStream.close();
            delete _currentSession;
            _currentSession = nullptr;
            _profileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            if (_profileCount++ > 0)
                _outputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            _outputStream << "{\"cat\":\"function\",\"dur\":";
            _outputStream << (result.End - result.Start) << ",\"name\":\"";
            _outputStream << name << "\",\"ph\":\"X\",\"pid\":0,\"tid\":";
            _outputStream << result.ThreadID << ",\"ts\":";
            _outputStream << result.Start;
            _outputStream << "}";

            _outputStream.flush();
        }

        void WriteHeader()
        {
            _outputStream << "{\"otherData\": {},\"traceEvents\":[";
            _outputStream.flush();
        }

        void WriteFooter()
        {
            _outputStream << "]}";
            _outputStream.flush();
        }

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }
    private:
        InstrumentationSession* _currentSession;
        std::ofstream _outputStream;
        int _profileCount;
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name)
            : _pName(name), _isStopped(false)
        {
            _startTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!_isStopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ _pName, start, end, threadID });

            _isStopped = true;
        }
    private:
        const char* _pName;
        std::chrono::time_point<std::chrono::high_resolution_clock> _startTimepoint;
        bool _isStopped;
    };
}

#define BG_PROFILE 0

#if BG_PROFILE
#define BG_PROFILE_BEGIN_SESSION(name, filepath) ::Bagel::Instrumentor::Get().BeginSession(name, filepath)
#define BG_PROFILE_END_SESSION() ::Bagel::Instrumentor::Get().EndSession();
#define BG_PROFILE_SCOPE(name) ::Bagel::InstrumentationTimer timer##__LINE__(name);
#define BG_PROFILE_FUNCTION() BG_PROFILE_SCOPE(__FUNCSIG__)
#else
#define BG_PROFILE_BEGIN_SESSION(name, filepath)
#define BG_PROFILE_END_SESSION()
#define BG_PROFILE_FUNCTION()
#define BG_PROFILE_SCOPE(name)
#endif

#define BG_PROFILE_RENDERER 1

#if BG_PROFILE_RENDERER
#define BG_PROFILE_RENDERER_BEGIN_SESSION(name, filepath) BG_PROFILE_BEGIN_SESSION(name,filepath)
#define BG_PROFILE_RENDERER_END_SESSION() BG_PROFILE_END_SESSION()
#define BG_PROFILE_RENDERER_SCOPE(name) BG_PROFILE_SCOPE(name);
#define BG_PROFILE_RENDERER_FUNCTION() BG_PROFILE_SCOPE(__FUNCSIG__)
#else
#define BG_PROFILE_RENDERER_BEGIN_SESSION(name, filepath)
#define BG_PROFILE_RENDERER_END_SESSION()
#define BG_PROFILE_RENDERER_FUNCTION()
#define BG_PROFILE_RENDERER_SCOPE(name)
#endif
