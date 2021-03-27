#include "Settings.h"

Settings kinectSettings = Settings();
// glm serialization realized in K2API

/* Serialize all variables from kinectSettings class */
template <class Archive>
void Settings::serialize(Archive& archive, const unsigned int version)
{
	// Archive all the data (needs to be added manually)
	archive& BOOST_SERIALIZATION_NVP(waistOrientationTrackingOption)
		& BOOST_SERIALIZATION_NVP(feetOrientationTrackingOption)
		& BOOST_SERIALIZATION_NVP(globalPositionTrackingFilterOption)
		& BOOST_SERIALIZATION_NVP(playspaceOrigin)
		& BOOST_SERIALIZATION_NVP(rotationMatrix)
		& BOOST_SERIALIZATION_NVP(translationVector)
		& BOOST_SERIALIZATION_NVP(radPlayspaceOffset)
		& BOOST_SERIALIZATION_NVP(trackerVector)
		& BOOST_SERIALIZATION_NVP(flipSkeleton);
}

/* Save settings with boost and output file stream */
void Settings::saveSettings()
{
	try
	{
		std::ofstream output("KinectToVR_settings.xml");

		boost::archive::xml_oarchive archive(output);
		archive << BOOST_SERIALIZATION_NVP(kinectSettings);
		LOG(INFO) << "Settings have been saved to file \"KinectToVR_settings.xml\"";
	}
	catch (boost::archive::archive_exception const& e)
	{
		LOG(ERROR) << "Archive serialization error: " << e.what();
	}
}

/* Read class from input file stream */
void Settings::readSettings()
{
	try {
		std::ifstream input("KinectToVR_settings.xml");

		boost::archive::xml_iarchive archive(input);
		archive >> BOOST_SERIALIZATION_NVP(kinectSettings);
		LOG(INFO) << "Settings have been saved to file \"KinectToVR_settings.xml\"";
	}
	catch (boost::archive::archive_exception const& e)
	{
		LOG(ERROR) << "Archive serialization error: " << e.what();
	}
}
