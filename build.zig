const Target = @import("std").build.Target;
const CrossTarget = @import("std").build.CrossTarget;
const LibExeObjStep = @import("std").build.LibExeObjStep;
const Builder = @import("std").build.Builder;
const builtin = @import("builtin");
const warn = @import("std").debug.warn;

const target = Target{
    .Cross = CrossTarget{
        .arch = builtin.Arch.x86_64,
        .os = builtin.Os.windows,
        .abi = builtin.Abi.gnu,
    },
};

const include_dirs = [_][]const u8{"include"};

const system_libraries = [_][]const u8{"c"};

const c_sources = struct {
    const general_sources = [_][]const u8{"src/buffer.c"};

    const main_sources = general_sources ++ [_][]const u8{"src/main.c"};

    const test_sources = general_sources ++ [_][]const u8{"test/test.c"};
};

const c_flags = struct {
    const general_flags = [_][]const u8{
        "-std=c11",
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Wwrite-strings",
        "-no-pie",
        "-fno-pie",
    };

    const release_flags = general_flags;

    const debug_flags = general_flags ++ [_][]const u8{"-Wno-unused-parameter"};

    const test_flags = debug_flags;
};

fn buildMainProgram(b: *Builder) void {
    const mode = b.standardReleaseOptions();

    const exe: *LibExeObjStep = b.addExecutable("hedit", null);
    exe.setBuildMode(mode);

    if (b.is_release) {
        exe.strip = true;
    }

    exe.single_threaded = true;

    exe.setTheTarget(target);

    for (c_sources.main_sources) |source| {
        if (b.is_release) {
            exe.addCSourceFile(source, c_flags.release_flags);
        } else {
            exe.addCSourceFile(source, c_flags.debug_flags);
        }
    }

    for (include_dirs) |include_dir| {
        exe.addIncludeDir(include_dir);
    }

    for (system_libraries) |library| {
        exe.linkSystemLibrary(library);
    }
    exe.install();

    // Run step.
    const run_cmd = exe.run();
    const run_step = b.step("run", "Run the program!");
    run_step.dependOn(&run_cmd.step);
}

fn buildTestProgram(b: *Builder) void {
    const mode = builtin.Mode.Debug;

    const exe: *LibExeObjStep = b.addExecutable("hedit_test", null);
    exe.setBuildMode(mode);

    exe.single_threaded = true;

    exe.setTheTarget(target);

    for (c_sources.test_sources) |source| {
        exe.addCSourceFile(source, c_flags.test_flags);
    }

    for (include_dirs) |include_dir| {
        exe.addIncludeDir(include_dir);
    }

    for (system_libraries) |library| {
        exe.linkSystemLibrary(library);
    }
    exe.install();

    // Run step.
    const run_cmd = exe.run();
    const run_step = b.step("test", "Test the program!");
    run_step.dependOn(&run_cmd.step);
}

pub fn build(b: *Builder) void {
    buildMainProgram(b);
    buildTestProgram(b);
}
