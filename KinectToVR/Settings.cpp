#include "Settings.h"
#include <boost_serialization_eigen.h>

Settings kinectSettings = Settings();

/* Addons to serialize types not supported natively by boost */
namespace boost::serialization {
	template <typename Ar>
	void serialize(Ar& ar, glm::vec3& v, unsigned) {
		ar& make_nvp("x", v.x)& make_nvp("y", v.y)& make_nvp("z", v.z);
	}
	template <typename Ar>
	void serialize(Ar& ar, glm::quat& q, unsigned) {
		ar& make_nvp("w", q.w)& make_nvp("x", q.x)& make_nvp("y", q.y)& make_nvp("z", q.z);
	}
}

/* Serialize all variables from kinectsettings class */
template<class Archive> 
void Settings::serialize(Archive& archive, const unsigned int version) {
	archive& waistOrientationTrackingOption
		& feetOrientationTrackingOption
		& positionalTrackingFilterOption
		& playspaceOrigin
		& rotationMatrix
		& translationVector
		& footRightOffset& footRightRotOffset
		& footLeftOffset& footLeftRotOffset
		& waistOffset& waistRotOffset
		& radPlayspaceOffset;
}

/* Save settings with boost and output file stream */
void Settings::saveSettings() {
	std::ofstream output("settings.cfg");
	
	boost::archive::text_oarchive archive(output);
	archive << kinectSettings;
}

/* Read class from input file stream */
void Settings::readSettings() {
	std::ifstream input("settings.cfg");

	boost::archive::text_iarchive archive(input);
	archive >> kinectSettings;
}
