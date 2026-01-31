// nova_cli.h
#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

    enum BuildMode : uint8_t {
        BUILD_MODE_DEBUG,
        BUILD_MODE_DEV,
        BUILD_MODE_RELEASE,
        BUILD_MODE_FAST,
        BUILD_MODE_TEST
    };

    struct NovaSlice;

    enum Errors : uint8_t {
        ERROR_NONE = 0,                // Success
        ERROR_MISSING_INPUT_FILE,
        ERROR_INVALID_INPUT_FILE,
        ERROR_TOO_MANY_INPUT_FILES,
        ERROR_INVALID_BUILD_MODE,
        ERROR_CONFLICTING_OPTIONS,
        ERROR_HELP_REQUESTED,
        ERROR_VERSION_REQUESTED,
        ERROR_OUT_OF_MEMORY,
        ERROR_INTERNAL_ERROR
    };

    struct Options {
        BuildMode mode;
        bool show_version;
        const char* iFile;  // null if not provided
    };

    // Converts string like "release" → BUILD_MODE_RELEASE
    // Returns BUILD_MODE_DEBUG on invalid input (or handle via Errors)
    BuildMode string_to_build_mode(const NovaSlice* mode_str);

    // Result type:
    // - If error == ERROR_NONE → options is valid
    // - Otherwise → ignore options, check error
    typedef struct {
        Options options;
        Errors error;
    } Result;

    // Helper: was parsing successful?
    static inline bool ok(const Result* r) {
        return r->error == ERROR_NONE;
    }

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class CoreInterfaceCommand {
public:
    CoreInterfaceCommand() = delete;
    CoreInterfaceCommand(const CoreInterfaceCommand&) = delete;
    CoreInterfaceCommand& operator=(const CoreInterfaceCommand&) = delete;

    static Result parse(int argc, const char** argv);
};
#endif