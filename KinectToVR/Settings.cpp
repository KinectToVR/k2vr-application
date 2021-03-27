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
		& BOOST_SERIALIZATION_NVP(flipSkeleton);
}

/* Save settings with boost and output file stream */
void Settings::saveSettings()
{
	std::ofstream output("settings.cfg");

	/* Data needs to be stored in other class
	 * 'cause Eigen doesn't have full-time serialization for now */
	for (int i = 0; i < 3; i++)
		glOrientationOffsets[i] = p_cast_type<glm::quat>(orientationOffsets[i]);

	boost::archive::text_oarchive archive(output);
	archive << BOOST_SERIALIZATION_NVP(kinectSettings);
}

/* Read class from input file stream */
void Settings::readSettings()
{
	std::ifstream input("settings.cfg");

	boost::archive::text_iarchive archive(input);
	archive >> BOOST_SERIALIZATION_NVP(kinectSettings);

	/* Data needs to be stored in other class
	 * 'cause Eigen doesn't have full-time serialization for now */
	for (int i = 0; i < 3; i++)
		orientationOffsets[i] = p_cast_type<Eigen::Quaternionf>(glOrientationOffsets[i]);
}
