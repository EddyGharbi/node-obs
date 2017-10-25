// Load node-obs
var obs_module = require('../node-obs/node-obs');

var colors = require('colors/safe');
var sleep = require('sleep');
var failed = 0;
var succeed = 0;

function runTest(resultTest) {
	if(resultTest.localeCompare("SUCCESS") == 0) {
		succeed++;
		return colors.green(resultTest);
	}
	else {
		failed++;
		return colors.red(resultTest);
	}
}

console.log(colors.blue("####### ") + "Ready to test obs module" + colors.blue(" #######"));

//OBS_API
console.log(colors.blue("OBS_API_test_initOBS_API : ") + runTest(obs_module.OBS_API_test_initOBS_API()));
console.log(colors.blue("OBS_API_test_openAllModules : ") + runTest(obs_module.OBS_API_test_openAllModules()));
console.log(colors.blue("OBS_API_test_initAllModules : ") + runTest(obs_module.OBS_API_test_initAllModules()));

//OBS_service
console.log(colors.blue("OBS_service_test_resetAudioContext : ") + runTest(obs_module.OBS_service_test_resetAudioContext()));
console.log(colors.blue("OBS_service_test_resetVideoContext : ") + runTest(obs_module.OBS_service_test_resetVideoContext()));
console.log(colors.blue("OBS_service_test_createAudioEncoder : ") + runTest(obs_module.OBS_service_test_createAudioEncoder()));
console.log(colors.blue("OBS_service_test_createVideoStreamingEncoder : ") + runTest(obs_module.OBS_service_test_createVideoStreamingEncoder()));
console.log(colors.blue("OBS_service_test_createVideoRecordingEncoder : ") + runTest(obs_module.OBS_service_test_createVideoRecordingEncoder()));
console.log(colors.blue("OBS_service_test_createService : ") + runTest(obs_module.OBS_service_test_createService()));
console.log(colors.blue("OBS_service_test_createRecordingSettings : ") + runTest(obs_module.OBS_service_test_createRecordingSettings()));
console.log(colors.blue("OBS_service_test_createStreamingOutput : ") + runTest(obs_module.OBS_service_test_createStreamingOutput()));
console.log(colors.blue("OBS_service_test_createRecordingOutput : ") + runTest(obs_module.OBS_service_test_createRecordingOutput()));
//console.log(colors.blue("OBS_service_test_startStreaming : ") + runTest(obs_module.OBS_service_test_startStreaming()));
//console.log(colors.blue("OBS_service_test_startRecording : ") + runTest(obs_module.OBS_service_test_startRecording()));
//console.log(colors.blue("OBS_service_test_stopStreaming : ") + runTest(obs_module.OBS_service_test_stopStreaming()));
//console.log(colors.blue("OBS_service_test_stopRecording : ") + runTest(obs_module.OBS_service_test_stopRecording()));
console.log(colors.blue("OBS_service_test_associateAudioAndVideoToTheCurrentStreamingContext : ") + runTest(obs_module.OBS_service_test_associateAudioAndVideoToTheCurrentStreamingContext()));
console.log(colors.blue("OBS_service_test_associateAudioAndVideoToTheCurrentRecordingContext : ") + runTest(obs_module.OBS_service_test_associateAudioAndVideoToTheCurrentRecordingContext()));
console.log(colors.blue("OBS_service_test_associateAudioAndVideoEncodersToTheCurrentStreamingOutput : ") + runTest(obs_module.OBS_service_test_associateAudioAndVideoEncodersToTheCurrentStreamingOutput()));
console.log(colors.blue("OBS_service_test_associateAudioAndVideoEncodersToTheCurrentRecordingOutput : ") + runTest(obs_module.OBS_service_test_associateAudioAndVideoEncodersToTheCurrentRecordingOutput()));
console.log(colors.blue("OBS_service_test_setServiceToTheStreamingOutput : ") + runTest(obs_module.OBS_service_test_setServiceToTheStreamingOutput()));
console.log(colors.blue("OBS_service_test_setRecordingSettings : ") + runTest(obs_module.OBS_service_test_setRecordingSettings()));

//OBS_content
console.log(colors.blue("OBS_content_test_createScene : ") + runTest(obs_module.OBS_content_test_createScene()));
console.log(colors.blue("OBS_content_test_removeScene : ") + runTest(obs_module.OBS_content_test_removeScene()));
console.log(colors.blue("OBS_content_test_getListCurrentScenes : ") + runTest(obs_module.OBS_content_test_getListCurrentScenes()));
console.log(colors.blue("OBS_content_test_getListCurrentSourcesFromScene : ") + runTest(obs_module.OBS_content_test_getListCurrentSourcesFromScene()));
console.log(colors.blue("OBS_content_test_getListInputSources : ") + runTest(obs_module.OBS_content_test_getListInputSources()));
console.log(colors.blue("OBS_content_test_getListFilters : ") + runTest(obs_module.OBS_content_test_getListFilters()));
console.log(colors.blue("OBS_content_test_getListTransitions : ") + runTest(obs_module.OBS_content_test_getListTransitions()));
console.log(colors.blue("OBS_content_test_addSource : ") + runTest(obs_module.OBS_content_test_addSource()));
console.log(colors.blue("OBS_content_test_removeSource : ") + runTest(obs_module.OBS_content_test_removeSource()));
//console.log(colors.blue("OBS_content_test_getSourceFrame : ") + runTest(obs_module.OBS_content_test_getSourceFrame()));
console.log(colors.blue("OBS_content_test_getSourceProperties : ") + runTest(obs_module.OBS_content_test_getSourceProperties()));
//console.log(colors.blue("OBS_content_test_getSourcePropertiesSubParameters : ") + runTest(obs_module.OBS_content_test_getSourcePropertiesSubParameters()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_boolType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_boolType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_colorType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_colorType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_intType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_intType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_floatType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_floatType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_textType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_textType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_fontType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_fontType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_pathType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_pathType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_buttonType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_buttonType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_editableListType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_editableListType()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_listType_intFormat : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_listType_intFormat()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_listType_floatFormat : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_listType_floatFormat()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_listType_stringFormat : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_listType_stringFormat()));
console.log(colors.blue("OBS_content_test_getSourcePropertyCurrentValue_frameRateType : ") + runTest(obs_module.OBS_content_test_getSourcePropertyCurrentValue_frameRateType()));
console.log(colors.blue("OBS_content_test_setProperty : ") + runTest(obs_module.OBS_content_test_setProperty()));
console.log(colors.blue("OBS_content_test_setSourcePosition : ") + runTest(obs_module.OBS_content_test_setSourcePosition()));
console.log(colors.blue("OBS_content_test_setSourceScaling : ") + runTest(obs_module.OBS_content_test_setSourceScaling()));
console.log(colors.blue("OBS_content_test_setSourceOrder : ") + runTest(obs_module.OBS_content_test_setSourceOrder()));

//OBS_settings
// console.log(colors.blue("OBS_settings_test_getGeneralSettings : ") + runTest(obs_module.OBS_settings_test_getGeneralSettings()));
// console.log(colors.blue("OBS_settings_test_saveGeneralSettings : ") + runTest(obs_module.OBS_settings_test_saveGeneralSettings()));
/*console.log(colors.blue("OBS_settings_test_getStreamSettings : ") + runTest(obs_module.OBS_settings_test_getStreamSettings()));
console.log(colors.blue("OBS_settings_test_saveStreamSettings : ") + runTest(obs_module.OBS_settings_test_saveStreamSettings()));
console.log(colors.blue("OBS_settings_test_getStreamTypesForStreamSettings : ") + runTest(obs_module.OBS_settings_test_getStreamTypesForStreamSettings()));
console.log(colors.blue("OBS_settings_test_setStreamTypeForStreamSettings : ") + runTest(obs_module.OBS_settings_test_setStreamTypeForStreamSettings()));*/
// console.log(colors.blue("OBS_settings_test_getOutputSimpleStreamingSettings : ") + runTest(obs_module.OBS_settings_test_getOutputSimpleStreamingSettings()));
// console.log(colors.blue("OBS_settings_test_saveOutputSimpleStreamingSettings : ") + runTest(obs_module.OBS_settings_test_saveOutputSimpleStreamingSettings()));
/*console.log(colors.blue("OBS_settings_test_getOutputSimpleRecordingSettings : ") + runTest(obs_module.OBS_settings_test_getOutputSimpleRecordingSettings()));
console.log(colors.blue("OBS_settings_test_saveOutputSimpleRecordingSettings : ") + runTest(obs_module.OBS_settings_test_saveOutputSimpleRecordingSettings()));
console.log(colors.blue("OBS_settings_test_getOutputAdvancedStreamingSettings : ") + runTest(obs_module.OBS_settings_test_getOutputAdvancedStreamingSettings()));
console.log(colors.blue("OBS_settings_test_saveOutputAdvancedStreamingSettings : ") + runTest(obs_module.OBS_settings_test_saveOutputAdvancedStreamingSettings()));
console.log(colors.blue("OBS_settings_test_getOutputAdvancedRecordingSettings : ") + runTest(obs_module.OBS_settings_test_getOutputAdvancedRecordingSettings()));
console.log(colors.blue("OBS_settings_test_saveOutputAdvancedRecordingSettings : ") + runTest(obs_module.OBS_settings_test_saveOutputAdvancedRecordingSettings()));
console.log(colors.blue("OBS_settings_test_getOutputAdvancedAudioSettings : ") + runTest(obs_module.OBS_settings_test_getOutputAdvancedAudioSettings()));
console.log(colors.blue("OBS_settings_test_saveOutputAdvancedAudioSettings : ") + runTest(obs_module.OBS_settings_test_saveOutputAdvancedAudioSettings()));
console.log(colors.blue("OBS_settings_test_getAudioSettings : ") + runTest(obs_module.OBS_settings_test_getAudioSettings()));
console.log(colors.blue("OBS_settings_test_saveAudioSettings : ") + runTest(obs_module.OBS_settings_test_saveAudioSettings()));
console.log(colors.blue("OBS_settings_test_getHotkeysSettings : ") + runTest(obs_module.OBS_settings_test_getHotkeysSettings()));
console.log(colors.blue("OBS_settings_test_saveHotkeysSettings : ") + runTest(obs_module.OBS_settings_test_saveHotkeysSettings()));
console.log(colors.blue("OBS_settings_test_getAdvancedSettings : ") + runTest(obs_module.OBS_settings_test_getAdvancedSettings()));
console.log(colors.blue("OBS_settings_test_saveAdvancedSettings : ") + runTest(obs_module.OBS_settings_test_saveAdvancedSettings()));*/

var numberOfTests = failed + succeed;

console.log(colors.blue("####### ") + "End of unit tests" + colors.blue(" #######"));
console.log("Number of succeed tests : " + colors.green(succeed) + "/" + numberOfTests);
console.log("Number of failed tests : " + colors.red(failed) + "/" + numberOfTests);

console.log("Exiting process");

// console.log(obs_module.OBS_settings_getGeneralSettings());
// console.log(obs_module.OBS_settings_getGeneralSettings()[0]);
// console.log(obs_module.OBS_settings_getGeneralSettings()[0].nameSubCategory);
// console.log(obs_module.OBS_settings_getGeneralSettings()[0].parameters[0]);
// console.log(obs_module.OBS_settings_getGeneralSettings()[0]);
// console.log(obs_module.OBS_settings_getGeneralSettings()[1].parameters[0]);
// obs_module.OBS_settings_saveGeneralSettings(obs_module.OBS_settings_getGeneralSettings());

var generalSettings = obs_module.OBS_settings_getSettings('General');
var streamSettings = obs_module.OBS_settings_getSettings('Stream');
var outputSettings = obs_module.OBS_settings_getSettings('Output');
var audioSettings = obs_module.OBS_settings_getSettings('Audio');
var videoSettings = obs_module.OBS_settings_getSettings('Video');


console.log(videoSettings);
// console.log(audioSettings[0].parameters);
// obs_module.OBS_settings_saveSettings('Audio', audioSettings);

// console.log(outputSettings[1].parameters[0].currentValue);
// outputSettings[1].parameters[0].currentValue = 2000;
// console.log(outputSettings[1].parameters[0].currentValue);

// obs_module.OBS_settings_saveSettings('Output', outputSettings);
// console.log(obs_module.OBS_settings_getSettings('Output')[1].parameters[0].currentValue);


// console.log(streamSettings[1].parameters[2][2]);



// console.log(outputSettings[0].parameters[0]);


// console.log(outputSettings[1].parameters[0]);
// console.log(outputSettings[1].parameters[1]);
// console.log(outputSettings[1].parameters[2]);
// console.log(outputSettings[1].parameters[3]);
// console.log(outputSettings[1].parameters[4]);
// console.log(outputSettings[1].parameters[5]);
// console.log(outputSettings[0].parameters[0]);
// console.log(outputSettings[0].parameters[1]);
// console.log(outputSettings[0].parameters[2]);
// console.log(outputSettings[0].parameters[3]);
// console.log(outputSettings[0].parameters[4]);
// console.log(outputSettings[0].parameters[5]);
// console.log(outputSettings[0].parameters[6]);

// console.log(generalSettings[1].parameters[0]);
// console.log(streamSettings);

// console.log(obs_module.OBS_settings_getGeneralSettings()[1].parameters[0]);
// console.log(obs_module.OBS_settings_getGeneralSettings()[1].parameters[0][2].value);

// generalSettings[1].parameters[0][2].value = 0;
// console.log(generalSettings[1].parameters[0][2].value);
// obs_module.OBS_settings_saveSettings('General', generalSettings);

// console.log(obs_module.OBS_settings_getSettings('General')[1].parameters[0][2].value);

// obs_module.OBS_settings_saveGeneralSettings(generalSettings);
// obs_module.OBS_settings_saveGeneralSettings(streamSettings);

// console.log(streamSettings[1].parameters[2]);

// service = [		
// 			{ name: 'service'},
// 			{ type: 'OBS_COMBO_FORMAT_STRING'},
// 			{ value: {'YouTube / YouTube Gaming': 'YouTube / YouTube Gaming'}},
// 			{ description: 'Service'},
// 			{ visible: 1},
// 			{ enabled: 1}
// 		]

// newStreamSettings = {
// 	// [
// 		nameSubCategory: 'Untitled',
// 		parameters: [ 
// 			service,
// 			service
// 		]
// 	// ]
// 	// {
// 	// 	nameSubCategory: 'Untitled',
// 	// 	parameters:
// 	// }
// }

// console.log(streamSettings);
// console.log("----------------");
// console.log(streamSettings[1].parameters[0][2]);
// console.log("----------------");
// console.log(streamSettings[1].parameters[0]);
// console.log(streamSettings[1].parameters[1]);
// console.log(streamSettings[1].parameters[2]);
// console.log(streamSettings[1].parameters[3]);

// console.log(obs_module.OBS_settings_getListCategories());

process.exit();

