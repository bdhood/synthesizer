import json, os, sys, subprocess, time
from builder import gcc

if __name__ == "__main__":
    print("Starting build, target '" + sys.platform + "'...\n")
    exe_name = "synthesizer.exe"
    os.system("taskkill /F /IM " + exe_name)
    gcc = gcc.GCC_WRAPPER()
    gcc.src_files = [
        "asiosdk2/common/asio.cpp",
        "asiosdk2/host/asiodrivers.cpp",
        "asiosdk2/host/pc/asiolist.cpp",
        "portaudio/src/common/pa_allocation.c",
        "portaudio/src/common/pa_converters.c",
        "portaudio/src/common/pa_cpuload.c",
        "portaudio/src/common/pa_dither.c",
        "portaudio/src/common/pa_front.c",
        "portaudio/src/common/pa_process.c",
        "portaudio/src/common/pa_ringbuffer.c",
        "portaudio/src/common/pa_stream.c",
        "portaudio/src/common/pa_trace.c",
        "portaudio/src/hostapi/asio/pa_asio.cpp",
        "portaudio/src/os/win/pa_win_coinitialize.c",
        "portaudio/src/os/win/pa_win_hostapis.c",
        "portaudio/src/os/win/pa_win_util.c",
        "portaudio/src/os/win/pa_win_waveformat.c",
        "portaudio/src/os/win/pa_x86_plain_converters.c",
        "synthesizer/components/mixer.c",
        "synthesizer/components/normalizer.c",
        "synthesizer/components/synth.c",
        "synthesizer/instruments/saw.c",
        "synthesizer/instruments/sine.c",
        "synthesizer/instruments/square.c",
        "synthesizer/interface/interface.c",
        "synthesizer/interface/controls/button.c",
        "synthesizer/interface/controls/label.c",
        "synthesizer/interface/controls/groupbox.c",
        "synthesizer/interface/controls/waveform.c",
        "synthesizer/keyboard/activekeys.c",
        "synthesizer/keyboard/activenotes.c",
        "synthesizer/keyboard/keyboard.c",
        "synthesizer/keyboard/keymap.c",
        "synthesizer/music/freqmap.c",
        "synthesizer/music/transpose.c",
        "synthesizer/synthesizer.cpp"
    ]
    gcc.incl_dirs = [
        "asiosdk2/common",
        "asiosdk2/host",
        "portaudio/include",
        "portaudio/src/common",
        "portaudio/src/os/win",
        "synthesizer/components",
        "synthesizer/instruments",
        "synthesizer/interface",
        "synthesizer/interface/controls",
        "synthesizer/keyboard",
        "synthesizer/music"
    ]
    gcc.libs = [
        "gdi32",
        "ole32",
        "winmm"
    ]
    gcc.output = "bin/" + exe_name
    gcc.build()


    process = subprocess.Popen(gcc.output)
    while process.poll() == None:
        time.sleep(0.25)


