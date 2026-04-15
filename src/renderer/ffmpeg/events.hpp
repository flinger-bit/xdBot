#pragma once

#include "render_settings.hpp"

#include <filesystem>
#include <functional>
#include <utility>
#include <vector>

namespace ffmpeg::events {
namespace impl {

struct Dummy {};

class CreateRecorderEvent {
public:
    CreateRecorderEvent() = default;

    void post() {
        if (m_ptr == nullptr) {
            m_ptr = new Dummy();
        }
    }

    void setPtr(void* ptr) { m_ptr = ptr; }
    void* getPtr() const { return m_ptr; }

private:
    void* m_ptr = nullptr;
};

class DeleteRecorderEvent {
public:
    explicit DeleteRecorderEvent(void* ptr) : m_ptr(ptr) {}

    void post() {
        if (m_ptr != nullptr) {
            delete static_cast<Dummy*>(m_ptr);
            m_ptr = nullptr;
        }
    }

    void* getPtr() const { return m_ptr; }

private:
    void* m_ptr = nullptr;
};

class InitRecorderEvent {
public:
    InitRecorderEvent(void* ptr, RenderSettings const* settings)
      : m_ptr(ptr), m_renderSettings(settings) {}

    void post() {
        m_result = geode::Err("Recorder backend is not implemented in this build.");
    }

    void setResult(geode::Result<>&& result) { m_result = std::move(result); }
    geode::Result<> getResult() { return m_result; }

    void* getPtr() const { return m_ptr; }
    RenderSettings const& getRenderSettings() const { return *m_renderSettings; }

private:
    RenderSettings const* m_renderSettings = nullptr;
    void* m_ptr = nullptr;
    geode::Result<> m_result = geode::Err("Event was not handled");
};

class StopRecorderEvent {
public:
    explicit StopRecorderEvent(void* ptr) : m_ptr(ptr) {}

    void post() {}

    void* getPtr() const { return m_ptr; }

private:
    void* m_ptr = nullptr;
};

class GetWriteFrameFunctionEvent {
public:
    struct DummyRecorder {
        geode::Result<> writeFrame(std::vector<std::uint8_t> const&) { return geode::Err("Not implemented"); }
    };

    using writeFrame_t = geode::Result<>(Dummy::*)(std::vector<std::uint8_t> const&);

    GetWriteFrameFunctionEvent() = default;

    void post() {
        m_function = nullptr;
    }

    void setFunction(writeFrame_t function) { m_function = function; }
    writeFrame_t getFunction() const { return m_function; }

private:
    writeFrame_t m_function = nullptr;
};

class CodecRecorderEvent {
public:
    CodecRecorderEvent() = default;

    void post() {
        m_codecs.clear();
    }

    void setCodecs(std::vector<std::string>&& codecs) { m_codecs = std::move(codecs); }
    std::vector<std::string> const& getCodecs() const { return m_codecs; }

private:
    std::vector<std::string> m_codecs;
};

class MixVideoAudioEvent {
public:
    MixVideoAudioEvent(
        std::filesystem::path const& videoFile,
        std::filesystem::path const& audioFile,
        std::filesystem::path const& outputMp4File
    ) {
        m_videoFile = &videoFile;
        m_audioFile = &audioFile;
        m_outputMp4File = &outputMp4File;
    }

    void post() {
        m_result = geode::Err("FFmpeg muxing is not implemented in this build.");
    }

    void setResult(geode::Result<>&& result) { m_result = std::move(result); }
    geode::Result<> getResult() { return m_result; }

    std::filesystem::path const& getVideoFile() const { return *m_videoFile; }
    std::filesystem::path const& getAudioFile() const { return *m_audioFile; }
    std::filesystem::path const& getOutputMp4File() const { return *m_outputMp4File; }

private:
    std::filesystem::path const* m_videoFile = nullptr;
    std::filesystem::path const* m_audioFile = nullptr;
    std::filesystem::path const* m_outputMp4File = nullptr;
    geode::Result<> m_result = geode::Err("Event was not handled");
};

class MixVideoRawEvent {
public:
    MixVideoRawEvent(
        std::filesystem::path const& videoFile,
        std::vector<float> const& raw,
        std::filesystem::path const& outputMp4File
    ) {
        m_videoFile = &videoFile;
        m_raw = &raw;
        m_outputMp4File = &outputMp4File;
    }

    void post() {
        m_result = geode::Err("FFmpeg muxing is not implemented in this build.");
    }

    void setResult(geode::Result<>&& result) { m_result = std::move(result); }
    geode::Result<> getResult() { return m_result; }

    std::filesystem::path const& getVideoFile() const { return *m_videoFile; }
    std::vector<float> const& getRaw() const { return *m_raw; }
    std::filesystem::path const& getOutputMp4File() const { return *m_outputMp4File; }

private:
    std::filesystem::path const* m_videoFile = nullptr;
    std::vector<float> const* m_raw = nullptr;
    std::filesystem::path const* m_outputMp4File = nullptr;
    geode::Result<> m_result = geode::Err("Event was not handled");
};

} // namespace impl

class Recorder {
public:
    Recorder() {
        impl::CreateRecorderEvent createEvent;
        createEvent.post();
        m_ptr = static_cast<impl::Dummy*>(createEvent.getPtr());
    }

    ~Recorder() {
        if (m_ptr != nullptr) {
            impl::DeleteRecorderEvent deleteEvent(m_ptr);
            deleteEvent.post();
        }
    }

    bool isValid() const { return m_ptr != nullptr; }

    geode::Result<> init(RenderSettings const& settings) {
        impl::InitRecorderEvent initEvent(m_ptr, &settings);
        initEvent.post();
        return initEvent.getResult();
    }

    void stop() {
        if (m_ptr != nullptr) {
            impl::StopRecorderEvent(m_ptr).post();
        }
    }

    geode::Result<> writeFrame(std::vector<std::uint8_t> const& frameData) {
        static auto writeFrame = [] {
            impl::GetWriteFrameFunctionEvent event;
            event.post();
            return event.getFunction();
        }();

        if (!writeFrame) {
            return geode::Err("Failed to call writeFrame function.");
        }

        return std::invoke(writeFrame, m_ptr, frameData);
    }

    static std::vector<std::string> getAvailableCodecs() {
        impl::CodecRecorderEvent codecEvent;
        codecEvent.post();
        return codecEvent.getCodecs();
    }

private:
    impl::Dummy* m_ptr = nullptr;
};

class AudioMixer {
public:
    AudioMixer() = delete;

    static geode::Result<> mixVideoAudio(
        std::filesystem::path const& videoFile,
        std::filesystem::path const& audioFile,
        std::filesystem::path const& outputMp4File
    ) {
        impl::MixVideoAudioEvent mixEvent(videoFile, audioFile, outputMp4File);
        mixEvent.post();
        return mixEvent.getResult();
    }

    static geode::Result<> mixVideoRaw(
        std::filesystem::path const& videoFile,
        std::vector<float> const& raw,
        std::filesystem::path const& outputMp4File
    ) {
        impl::MixVideoRawEvent mixEvent(videoFile, raw, outputMp4File);
        mixEvent.post();
        return mixEvent.getResult();
    }
};

} // namespace ffmpeg::events
