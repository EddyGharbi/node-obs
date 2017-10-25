#include <string>
#include <sstream>
#include <iostream>

#include <node.h>
#include <obs.h>
#include <vector>
#include <thread>
#include <chrono>

#ifndef _WIN32
#include <unistd.h>
#endif

using namespace v8;
using namespace std;

vector<string> tabSources;
vector<obs_sceneitem_t*> tabItems;

const struct video_output_info *voi;

///Wrap function
bool initOBS_API(void) {
    //obs_startup("en-US", "/Users/eddygharbi/Library/Application Support/obs-studio/plugin_config", NULL);
    obs_startup("en-US", NULL, NULL);
    
    cout << "TEST !!!" << endl;
    return obs_initialized();
}

bool openAndInitAllModules(void) {
    cout << "INIIITTTTTTTT !!!!!!!" << endl;
    //Open and init all necessary modules
    const int numberOfModules = 14;
    const char* availableModules[numberOfModules] = {
        "text-freetype2",
        "rtmp-services",
        "obs-x264",
        "obs-transitions",
        "obs-outputs",
        "obs-filters",
        "obs-ffmpeg",
        "mac-vth264",
        "mac-syphon",
        "mac-decklink",
        "mac-capture",
        "mac-avcapture",
        "image-source",
        "coreaudio-encoder"};
    const char* fileExtension = ".so";
    const char* pathBinModules = "./lib/";
    const char* pathDataModules = "./data/obs-plugins/";
    obs_module_t *module;
    
    for (int i = 0; i < numberOfModules; i++) {
        string bin = string(pathBinModules) + string(availableModules[i]) + string(fileExtension);
        string  data = string(pathDataModules) + string(availableModules[i]);
        if (obs_open_module(&module, bin.c_str(), data.c_str()) != MODULE_SUCCESS) {
            //return false;
        }
        else {
            cout << "Module opened" << endl;
        }
        if (!obs_init_module(module)) {
            //return false;
            cout << "Module non initiazided" << endl;
        }
        else {
            cout << "Module initialized" << endl;
        }
        cout << "Current index " << availableModules[i] << " : " << i << endl;
    }
    return true;
}

bool createScenesAndSources(void) {
    //Create new scene and source
    obs_data_t* settingsImage = obs_data_create();
    
    //Audio capture
    //obs_scene_t* audioOutput = obs_scene_create("wasapi_output_capture");
    //obs_scene_t* audioInout = obs_scene_create("wasapi_input_capture");
    
    //Image capture
    obs_source_t* imageSource = obs_source_create("image_source", "image", settingsImage, nullptr);
    
    //obs_data_set_string(settingsImage, "file", "C:/Users/eddy/Pictures/hitman-suit-game-hd-wallpaper-1920x1080-1998.jpg");
    
    //Scene creation
    obs_scene_t* scene = obs_scene_create("scene");
    
    obs_sceneitem_t *itemScene = NULL;
    itemScene = obs_scene_add(scene, imageSource);
    return true;
}

bool resetAudioContext(void) {
    //Reset audio
    struct obs_audio_info ai;
    
    ai.samples_per_sec = 44100;
    ai.speakers = SPEAKERS_STEREO;
    
    return obs_reset_audio(&ai);
}

bool resetVideoContext(void) {
    cout << "Reset video start" << endl;
    //Reset video
    obs_video_info ovi;
    
    ovi.graphics_module = "./lib/libobs-opengl.so";
    ovi.fps_num = 30;
    ovi.fps_den = 1;
    ovi.base_width = 1920;
    ovi.base_height = 1080;
    ovi.output_width = 1280;
    ovi.output_height = 720;
    ovi.output_format = VIDEO_FORMAT_NV12;
    ovi.adapter = 0;
    ovi.gpu_conversion = true;
    ovi.colorspace = VIDEO_CS_601;
    ovi.range = VIDEO_RANGE_PARTIAL;
    ovi.scale_type = OBS_SCALE_BICUBIC;
    
    return obs_reset_video(&ovi);
}

obs_output_t* createOutput(void) {
    return obs_output_create("rtmp_output", "simple_stream", nullptr, nullptr);
}

obs_encoder_t* createVideoEncoder(void) {
    obs_data_t *h264Settings = obs_encoder_defaults("obs_x264");
    
    return obs_video_encoder_create("obs_x264",
                                    "streaming_h264", h264Settings, nullptr);
}

obs_encoder_t* createAudioEncoder(void) {
    obs_data_t *aacSettings = obs_data_create();
    obs_data_set_string(aacSettings, "rate_control", "CBR");
    obs_data_set_int(aacSettings, "bitrate", 160);
    obs_data_set_int(aacSettings, "samplerate", 44100);
    cout << "Creating audio encoder " << obs_encoder_get_display_name("CoreAudio_AAC") << endl << endl;
    return obs_audio_encoder_create("CoreAudio_AAC", "simple_aac", aacSettings, 0, nullptr);
}

void associate_AVencoders_with_CurrentContext(obs_encoder_t* videoEncoder, obs_encoder_t* audioEncoder) {
    obs_encoder_set_video(videoEncoder, obs_get_video());
    obs_encoder_set_audio(audioEncoder, obs_get_audio());
}

void updateOutputSettings(obs_output_t* output,
                          obs_encoder_t* videoEncoder, obs_encoder_t* audioEncoder) {
    
    obs_output_set_video_encoder(output, videoEncoder);
    obs_output_set_audio_encoder(output, audioEncoder, 0);
}

obs_service_t* createService(void) {
    const char *type;
    obs_data_t *data = obs_data_create_from_json_file_safe(
                                                           "./config/service.json", "bak");
    
    obs_data_set_default_string(data, "type", "rtmp_common");
    type = obs_data_get_string(data, "type");
    
    obs_data_t *settings = obs_data_get_obj(data, "settings");
    obs_data_t *hotkey_data = obs_data_get_obj(data, "hotkeys");
    
    return obs_service_create(type, "default_service", settings, hotkey_data);
}

bool startOutput(obs_output_t* output) {
    return obs_output_start(output);
}

bool startStreamingBlackScreen() {
    //Start obs-studio API and initializing context
    initOBS_API();
    
    //Load modules
    openAndInitAllModules();
    
    //Create scenes and sources associate with the output
    createScenesAndSources();
    
    //Create output
    obs_output_t *	streamOutput = createOutput();
    
    //Creating video encoder
    obs_encoder_t *h264Streaming = createVideoEncoder();
    
    //Creating audio encoder
    obs_encoder_t* aacEncoder = createAudioEncoder();
    
    //Reset audio
    resetAudioContext();
    
    //Reset video
    resetVideoContext();
    
    //Associate A/V encoders with current context
    associate_AVencoders_with_CurrentContext(h264Streaming, aacEncoder);
    
    //Load service
    obs_service_t *service = createService();
    
    //Update stream output settings
    updateOutputSettings(streamOutput, h264Streaming, aacEncoder);
    
    //Assign service to an ouput
    obs_output_set_service(streamOutput, service);
    
    //Start output
    return startOutput(streamOutput);
}

static void LoadAudioDevice(const char *name, int channel, obs_data_t *parent)
{
    obs_data_t *data = obs_data_get_obj(parent, name);
    if (!data)
        return;
    
    obs_source_t *source = obs_load_source(data);
    if (source) {
        obs_set_output_source(channel, source);
        obs_source_release(source);
    }
    
    obs_data_release(data);
}

void SourceLoaded(void* data, obs_source_t* source) {
    obs_scene_from_source(source);
    obs_source_addref(source);
}

bool EnumSources(void *data, obs_source_t *source)
{
    
    const char *name = obs_source_get_name(source);
    const char *id = obs_source_get_id(source);
    
    tabSources.push_back(name);
    
    return true;
}


static bool LogSceneItem(obs_scene_t*, obs_sceneitem_t *item, void*)
{
    obs_source_t *source = obs_sceneitem_get_source(item);
    const char *name = obs_source_get_name(source);
    const char *id = obs_source_get_id(source);

    blog(LOG_INFO, "    - source: '%s' (%s)", name, id);
    tabItems.push_back(item);
    return true;
}

void startStreamingFromConfigFile() {
    ///Start obs-studio API and initializing context
    initOBS_API();
    
    ///Load modules
    openAndInitAllModules();
    
    ///Create scenes and sources associate with the output
    createScenesAndSources();
    
    ///Create output
    obs_output_t *	streamOutput = createOutput();
    
    ///Creating video encoder
    obs_encoder_t *h264Streaming = createVideoEncoder();
    
    ///Creating audio encoder
    obs_encoder_t* aacEncoder = createAudioEncoder();
    
    ///Reset audio
    resetAudioContext();
    
    ///Reset video
    resetVideoContext();
    
    ///Create scenes and sources
    //Reset output source
    obs_set_output_source(0, nullptr);
    obs_set_output_source(1, nullptr);
    obs_set_output_source(2, nullptr);
    obs_set_output_source(3, nullptr);
    obs_set_output_source(4, nullptr);
    obs_set_output_source(5, nullptr);
    
    //Reading to config scenes file to load the last user's configuration
    const char* file = "./config/Untitled.json";
    obs_data_t *data = obs_data_create_from_json_file_safe(file, "bak");
    
    //Creating default transitions in case none are found in the config file
    obs_source_t *cut = obs_source_create_private("cut_transition", "Cut", NULL);
    //obs_source_t* fade = obs_source_create_private("fade_transition", "Fade", NULL);
    obs_set_output_source(0, cut);
    
    
    //Serializing scenes / sources / transitions from the config file
    obs_data_array_t *sceneOrder = obs_data_get_array(data, "scene_order");
    obs_data_array_t *sources = obs_data_get_array(data, "sources");
    obs_data_array_t *transitions = obs_data_get_array(data, "transitions");
    
    //Getting current configurations
    const char       *sceneName = obs_data_get_string(data,	"current_scene");
    const char       *programSceneName = obs_data_get_string(data, "current_program_scene");
    const char       *transitionName = obs_data_get_string(data, "current_transition");
    int				 newDuration = obs_data_get_int(data, "transition_duration");
    if (!newDuration)
        newDuration = 300;
    
    if (!transitionName)
        transitionName = obs_source_get_name(cut);
    
    obs_data_set_default_string(data, "name", "Untitled");
    
    const char       *name = obs_data_get_string(data, "name");
    obs_source_t     *curScene;
    obs_source_t     *curProgramScene;
    obs_source_t     *curTransition;
    
    
    if (!name || !*name)
        name = "Untitled";
    
    //Loading all audio devices
    LoadAudioDevice("DesktopAudioDevice1", 1, data);
    LoadAudioDevice("DesktopAudioDevice2", 2, data);
    LoadAudioDevice("AuxAudioDevice1", 3, data);
    LoadAudioDevice("AuxAudioDevice2", 4, data);
    LoadAudioDevice("AuxAudioDevice3", 5, data);
    
    //Loading all the sources that we previously created
    obs_load_sources(sources, SourceLoaded, nullptr);
    
    //Setting current scene
retryScene:
    curScene = obs_get_source_by_name(sceneName);
    curProgramScene = obs_get_source_by_name(programSceneName);
    
    
    /* if the starting scene command line parameter is bad at all,
     * fall back to original settings */
    if ((!curScene || !curProgramScene)) {
        sceneName = obs_data_get_string(data, "current_scene");
        programSceneName = obs_data_get_string(data,
                                               "current_program_scene");
        obs_source_release(curScene);
        obs_source_release(curProgramScene);
        goto retryScene;
    }
    
    if (!curProgramScene) {
        curProgramScene = curScene;
        obs_source_addref(curScene);
    }
    
    
    ///SetCurrentScene(curScene, true);
    obs_transition_set(cut, curScene);
    obs_source_release(cut);
    
    ///Associate A/V encoders with current context
    associate_AVencoders_with_CurrentContext(h264Streaming, aacEncoder);
    
    ///Load service
    obs_service_t *service = createService();
    
    ///Update stream output settings
    updateOutputSettings(streamOutput, h264Streaming, aacEncoder);
    
    
    ///Assign service to an ouput
    obs_output_set_service(streamOutput, service);

    obs_data_t * h264Settings = obs_encoder_get_settings(h264Streaming);
    obs_data_t * aacSettings = obs_encoder_get_settings(aacEncoder);
    
    obs_data_set_int(h264Settings, "bitrate", 100);
    obs_service_apply_encoder_settings(service,
                                       h264Settings, aacSettings);
    
    
    ///Assing audio and video context to the created output
    obs_output_set_media(streamOutput,
                         obs_get_video(), obs_get_audio());
    
    

    /*voi = video_output_get_info(obs_output_video(streamOutput));
    struct video_scale_info *info = {0};
    info->width = 200;
    info->height = 200;
    info->format = VIDEO_FORMAT_I420;
    /*info->colorspace = voi->colorspace;
    info->range = voi->range;
    //obs_output_set_video_conversion(streamOutput, info);
/*
    */

    obs_set_output_source(0, curScene);
    /*
     ///Rendering
     //HWND dummyHWND = ::CreateWindowA("STATIC", "dummy", WS_VISIBLE, 0, 0, 1000, 800, NULL, NULL, NULL, NULL);
     //::SetWindowTextA(dummyHWND, "Dummy Window!");
     
     gs_init_data info = {};
     //info.window.hwnd = dummyHWND;
     info.view = (id)window;
     info.cx = 500;
     info.yx = 500;
     info.format = GS_RGBA;
     info.zsformat = GS_ZS_NONE;
     
     obs_display_t *display = obs_display_create(&info);
     
     obs_display_add_draw_callback(display, RenderVideo, nullptr);*/
    
    
    ///Start output
    //obs_output_set_preferred_size(streamOutput, 720, 576);
    cout << "Width output video: " << obs_output_get_width(streamOutput) << endl;
    cout << "Height output video: " << obs_output_get_height(streamOutput) << endl;

    obs_source_t *sourceToUpdate = obs_get_source_by_name("Video Capture Device");
    obs_scene_t *scene = obs_scene_from_source(curScene);
    obs_scene_enum_items(scene, LogSceneItem, nullptr);

    struct vec2 pos;
    pos.x = 500;
    pos.y = 500;    
    struct vec2 scale;
    scale.x = 0.2;
    scale.y = 0.2;
    obs_sceneitem_set_scale(tabItems.at(0), &scale);

    obs_source_set_flags(sourceToUpdate, OBS_SOURCE_ASYNC_VIDEO);
    startOutput(streamOutput);
}

///Unit tests
void test_initOBS_API(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_initOBS_API : ";
    string result;
    if(initOBS_API()) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_openAndInitAllModules(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_openAndInitAllModules : ";
    string result;
    if(openAndInitAllModules()) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_createScenesAndSources(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_createScenesAndSources : ";
    string result;
    if(createScenesAndSources()) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_resetAudioContext(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_resetAudioContext : ";
    string result;
    if(resetAudioContext()) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_resetVideoContext(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_resetVideoContext : ";
    string result;
    if(resetVideoContext() == OBS_VIDEO_SUCCESS) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_createOutput(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_createOutput : ";
    string result;
    if(createOutput() != NULL) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_createVideoEncoder(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_createVideoEncoder : ";
    string result;
    if(createVideoEncoder() != NULL) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_createAudioEncoder(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_createAudioEncoder : ";
    string result;
    if(createAudioEncoder() != NULL) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_createService(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_createService : ";
    string result;
    if(createService() != NULL) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_startStreamingBlackScreen(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_startStreamingBlackScreen : ";
    string result;
    if(startStreamingBlackScreen()) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
    }
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

void test_startStreamingFromConfigFile(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_startStreamingFromConfigFile : ";
    string result = "Streaming ... ";
    startStreamingFromConfigFile();
    functionName += result;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}

uint8_t* convert_UYVY422_to_I420(uint8_t* source) {
    int width = 1280;
    int height = 720;
    int outputSize = width*height*1.5;
    
    uint8_t* output = new uint8_t[outputSize];
    
    int uOffset = width*height;
    int vOffset = width*height + (width*height)/4;
    
    int indexY = 0;
    int indexU = uOffset;
    int indexV = vOffset;
    int lineNumber = 0;
    int lineSize = width*2;
    
    
    for(int indexSource=0;indexSource<width*height*2;indexSource+=4) {
        output[indexY] = source[indexSource+1];
        indexY++;
        output[indexY] = source[indexSource+3];
        indexY++;
        if(lineNumber==0 || lineNumber%2==0)
        {
            output[indexU] = source[indexSource];
            indexU++;
            output[indexV] = source[indexSource+2];
            indexV++;
        }
        if(lineSize==indexSource) {
            lineNumber++;
            lineSize += width*2;
        }/**/
    }
    return output;
}

void test_getFrameFromSource(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    string functionName = "test_getFrameFromSource : ";
    string result;
    
    
    ///Rendering
    obs_enum_sources(EnumSources, nullptr);

    obs_source_t * videoCaptureSource = obs_get_source_by_name("Video Capture Device");
    if(videoCaptureSource == NULL) {
        cout << "NULL" << endl;
    }

    struct obs_source_frame * frameVideoCapture=NULL;
    obs_source_frame_init(frameVideoCapture, VIDEO_FORMAT_NV12, 1280, 720);
    
    obs_source_output_video(videoCaptureSource, frameVideoCapture);
    
    frameVideoCapture = obs_source_get_frame(videoCaptureSource);

    if(frameVideoCapture != NULL) {
        result = "SUCCESS";
    } else {
        result = "FAILURE";
        obs_source_release_frame(videoCaptureSource, frameVideoCapture);
		std::this_thread::sleep_for(std::chrono::microseconds(10));
		//usleep(10);
    }
    functionName += result;
    if(result == "SUCCESS") {
        //functionName = frameVideoCapture->data[0];
        //cout << "#############" << frameVideoCapture->width << "  " << frameVideoCapture->height << endl;
        //cout << "#############" << frameVideoCapture->format << endl;
        
        /*args.GetReturnValue().Set(ArrayBuffer::New(isolate, frameVideoCapture->data[0], frameVideoCapture->width * frameVideoCapture->height * 2));*/
        uint8_t* output = convert_UYVY422_to_I420(frameVideoCapture->data[0]);
        args.GetReturnValue().Set(ArrayBuffer::New(isolate, output, frameVideoCapture->width * frameVideoCapture->height * 1.5));
        delete output;
        obs_source_release_frame(videoCaptureSource, frameVideoCapture);
    }
    else {
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, "FAIL"));
    }
    //args.GetReturnValue().Set(String::NewFromUtf8(isolate, functionName.c_str()));
}
void test_getPercentageDroppedFrame (const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    cout << "Debug 0" << endl;
    const obs_output_t* currentOutput = obs_get_output_by_name("simple_stream");

    if(currentOutput == NULL){
        cout << "NULL output source !!" << endl;
    }
    else {
        cout << "Debug 1" << endl;
        int framesDropped = obs_output_get_frames_dropped(currentOutput);
        cout << "Debug 2" << endl;
        int totalFramesDisplayed = obs_output_get_total_frames(currentOutput);
        cout << "Debug 3 : " << totalFramesDisplayed << endl;

        double percentageFranesDropped =  0;

        if(totalFramesDisplayed != 0) 
        {
            double percentageFranesDropped = framesDropped * 100 / totalFramesDisplayed;
        }
        cout << "Debug 4" << endl;
        args.GetReturnValue().Set(Number::New(isolate, percentageFranesDropped));        
    }

}

/*void init(Local<Object> exports) {
    //Function
    NODE_SET_METHOD(exports, "test_initOBS_API", test_initOBS_API);
    NODE_SET_METHOD(exports, "test_openAndInitAllModules", test_openAndInitAllModules);
    NODE_SET_METHOD(exports, "test_createScenesAndSources", test_createScenesAndSources);
    NODE_SET_METHOD(exports, "test_resetAudioContext", test_resetAudioContext);
    NODE_SET_METHOD(exports, "test_resetVideoContext", test_resetVideoContext);
    NODE_SET_METHOD(exports, "test_createVideoEncoder", test_createVideoEncoder);
    NODE_SET_METHOD(exports, "test_createAudioEncoder", test_createAudioEncoder);
    NODE_SET_METHOD(exports, "test_createService", test_createService);
    NODE_SET_METHOD(exports, "test_startStreamingBlackScreen", test_startStreamingBlackScreen);
    NODE_SET_METHOD(exports, "test_startStreamingFromConfigFile", test_startStreamingFromConfigFile);
    NODE_SET_METHOD(exports, "test_getFrameFromSource", test_getFrameFromSource);
    NODE_SET_METHOD(exports, "test_getPercentageDroppedFrame", test_getPercentageDroppedFrame);
}

NODE_MODULE(addon, init)*/
