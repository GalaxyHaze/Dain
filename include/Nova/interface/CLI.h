// nova_cli.h
#pragma once
#include <cstdint>

    enum class BuildMode: uint8_t {
        debug,
        dev,
        release,
        fast,
        test
    };

    struct NovaSlice;
    //TODO: some errors from CLI parser
    enum class Errors: uint8_t {

    };

    struct Options{
        BuildMode mode;
        bool show_version;
        uint32_t len;
        const char* iFile;
    };

    BuildMode string_to_build_mode(NovaSlice* mode_str);
    struct Result {
        union  {
            Options opt;
            Errors error;
        };
        uint8_t success;
    };

    Result initResultSuccess(Result* dst, Options* opt);
    Result initResultFail(Result* dst, Errors err);
    void emplaceResultSuccess(Result* dst, Options* opt);
    void emplaceResultFail(Result* dst, Errors err);
    bool ok(Result* result);

    #ifdef __cplusplus
    class CoreInterfaceCommand {
    public:
        CoreInterfaceCommand() = delete;
        CoreInterfaceCommand(const CoreInterfaceCommand&) = delete;
        CoreInterfaceCommand& operator=(const CoreInterfaceCommand&) = delete;

        static Result parse(int argc, const char** argv);
    };



#endif