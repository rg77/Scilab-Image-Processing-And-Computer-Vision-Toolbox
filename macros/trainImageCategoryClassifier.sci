function classifier = trainImageCategoryClassifier(imgSets, bag)
// This function is used to train an image classifier.
//
// Calling Sequence
// classifier = trainImageCategoryClassifier(imgSets, bag)
//
// Parameters
// classifier: Image category classifier
// imgSets: Input imageSet to train the classifier on
// bag: The bagOfFeatures of the imageSet provided
//
// Description
// This function trains an image category classifier which can be used to predict categories of images given to it as input using the predict() function.
//
// Examples
// imgSet = imageSet(directory,'recursive');
// [trainingSet testSet] = partition(imgSet,[0.8]);
// bag = bagOfFeatures(trainingSet);
// categoryClassifier = trainImageCategoryClassifier(trainingSet, bag);
//
// Authors
//  Rohit Suri
//  Umang Agrawal

	temp = opencv_trainImageCategoryClassifier(imgSets, bag)

	classifier = struct("ClassifierLocation", temp(1), "BagofFeaturesLocation", temp(2), "Description", temp(3))

endfunction
