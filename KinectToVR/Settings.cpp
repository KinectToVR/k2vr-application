#include "Settings.h"

Settings kinectSettings = Settings();

/* Addons to serialize types not supported natively by boost */
namespace boost::serialization
{
	template <typename Ar>
	void serialize(Ar& ar, glm::vec3& v, unsigned)
	{
		ar & make_nvp("x", v.x) & make_nvp("y", v.y) & make_nvp("z", v.z);
	}

	template <typename Ar>
	void serialize(Ar& ar, glm::quat& q, unsigned)
	{
		ar & make_nvp("w", q.w) & make_nvp("x", q.x) & make_nvp("y", q.y) & make_nvp("z", q.z);
	}
}

/* Serialize all variables from kinectsettings class */
template <class Archive>
void Settings::serialize(Archive& archive, const unsigned int version)
{
	archive & BOOST_SERIALIZATION_NVP(waistOrientationTrackingOption)
		& BOOST_SERIALIZATION_NVP(feetOrientationTrackingOption)
		& BOOST_SERIALIZATION_NVP(positionalTrackingFilterOption)
		& BOOST_SERIALIZATION_NVP(playspaceOrigin)
		& BOOST_SERIALIZATION_NVP(rotationMatrix)
		& BOOST_SERIALIZATION_NVP(translationVector)
		& BOOST_SERIALIZATION_NVP(positionalOffsets)
		& BOOST_SERIALIZATION_NVP(glOrientationOffsets)
		& BOOST_SERIALIZATION_NVP(radPlayspaceOffset)
		& BOOST_SERIALIZATION_NVP(flipSkeleton);
}

/* Save settings with boost and output file stream */
void Settings::saveSettings()
{
	std::ofstream output("settings.cfg");

	/* lil hack since i doesn't have full serialization for eigen */
	for (int i = 0; i < 3; i++)
		glOrientationOffsets[i] = pExchangeQG(orientationOffsets[i]);

	boost::archive::text_oarchive archive(output);
	archive << BOOST_SERIALIZATION_NVP(kinectSettings);
}

/* Read class from input file stream */
void Settings::readSettings()
{
	std::ifstream input("settings.cfg");

	boost::archive::text_iarchive archive(input);
	archive >> BOOST_SERIALIZATION_NVP(kinectSettings);

	/* lil hack since i doesn't have full serialization for eigen */
	for (int i = 0; i < 3; i++)
		orientationOffsets[i] = pExchangeQE(glOrientationOffsets[i]);
}
