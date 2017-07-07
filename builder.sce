mode(-1);
lines(0);

function main_builder()

    TOOLBOX_NAME  = "imageprocessing";
    TOOLBOX_TITLE = "Image Processing Toolbox";
    toolbox_dir   = get_absolute_file_path("builder.sce");

    // Check Scilab's version
    // =============================================================================

    try
        v = getversion("scilab");
    catch
        error(gettext("Scilab 5.5 or more is required."));
    end

    if v(1) < 5 & v(2) < 3 then
        // new API in scilab 5.3
        error(gettext("Scilab 5.5 or more is required."));
    end

    // Check modules_manager module availability
    // =============================================================================

    if ~isdef("tbx_build_loader") then
        error(msprintf(gettext("%s module not installed."), "modules_manager"));
    end

    // Action
    // =============================================================================

    tbx_builder_macros(toolbox_dir);
    //tbx_builder_src(toolbox_dir);
    tbx_builder_gateway(toolbox_dir);
    //tbx_build_localization(TOOLBOX_NAME, toolbox_dir);
    tbx_builder_help(toolbox_dir);
    tbx_build_loader(TOOLBOX_NAME, toolbox_dir);
    tbx_build_cleaner(TOOLBOX_NAME, toolbox_dir);

endfunction
// =============================================================================
main_builder();
exec macros/imshow.sci;
exec macros/show.sci;
<<<<<<< HEAD
exec macros/IDCT.sci;
exec macros/TemplateMatcher.sci;
exec macros/bundleAdjustments.sci;
exec macros/isEpipoleInImage.sci;
exec macros/peopleDetector.sci;
exec macros/undistortImage.sci;
exec macros/CascadeObjectDetector.sci;
exec macros/IFFT.sci;
exec macros/pyramid.sci;
exec macros/GeometricShearer.sci;
exec macros/rectifyStereoImages.sci;
exec macros/bbox2points.sci;
exec macros/bboxOverlapRatio.sci;
exec macros/cameraMatrix.sci;
exec macros/extractLBPFeatures.sci;
exec macros/genCheckerboardPoints.sci;
exec macros/integralFilter.sci;
exec macros/integralImage.sci;
exec macros/integralKernel.sci;
exec macros/rotationVectorToMatrix.sci;
exec macros/bagOfFeatures.sci;
exec macros/corner.sci;
exec macros/detectCheckerboardPoints.sci;
exec macros/detectHarrisFeatures.sci;
exec macros/detectMinEigenFeatures.sci;
exec macros/evaluate.sci;
exec macros/imageSet.sci;
exec macros/imgSetToList.sci;
exec macros/imread.sci;
exec macros/mattolist.sci;
exec macros/ocr.sci;
exec macros/partition.sci;
exec macros/predict.sci;
exec macros/trainImageCategoryClassifier.sci;
exec macros/classifierToList.sci;
exec macros/bagStructToList.sci;
exec macros/binaryFeatures.sci;
exec macros/detectFASTFeatures.sci;
exec macros/detectMSERFeatures.sci;
exec macros/evaluateImageRetrieval.sci;
exec macros/extractFeatures.sci;
exec macros/matchFeatures.sci;
exec macros/indexImages.sci;
exec macros/invertedIndexImages.sci;
exec macros/retrieveImages.sci;
exec macros/detectSURFFeatures.sci;
exec macros/SURFPoints.sci;
exec macros/detectBRISKFeatures.sci;
exec macros/BRISKPoints.sci;
exec macros/stereoParameters.sci;
exec macros/checkCameraParam.sci;
exec macros/checkStereoParam.sci;
exec macros/checkCIE.sci;
exec macros/checkCEE.sci;
exec macros/checkCCE.sci;
exec macros/checkSCE.sci;
exec macros/displayErrors.sci;
exec macros/VideoReader.sci;
exec macros/has_frame.sci;
exec macros/readframe.sci;
exec macros/ForegroundDetector.sci;
exec macros/readForegroundMask.sci;
exec macros/lineToBorderPoints.sci;
exec macros/cvHoughLines.sci;
exec macros/cvMedian.sci;
exec macros/cvMean.sci;
exec macros/cvMinimum.sci;
exec macros/cvMaximum.sci;
exec macros/cvVariance.sci;
exec macros/cvStandardDeviation.sci;
exec macros/DCT.sci;
exec macros/Deinterlacer.sci;
exec macros/undistortPoints.sci;
exec macros/triangulatePoints.sci;
exec macros/blobAnalysis.sci;
exec macros/KalmanFilter.sci;
exec macros/showMatchedFeatures.sci;
exec macros/opticalFlowFarneback.sci;
exec macros/convolver.sci;
exec macros/extractHOGFeatures.sci;
exec macros/estimateGeometricTransform.sci;
=======
>>>>>>> 684ad098cee7d163efcd0722f19bc7b309764086
clear main_builder; // remove main_builder on stack
// =============================================================================
