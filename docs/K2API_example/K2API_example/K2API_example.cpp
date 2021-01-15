// K2API_example.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <KinectToVR_API.h>

int main()
{
	/*
	 * Connect to the K2Driver socket on port 7135
	 * If connection succeed, we are expecting return code 0
	 */
	
	std::cout << "Initializing server...\n";
	
	if(k2_api::init_socket(7135) == 0)
		std::cout << "Initializing server succeed!\n";
	else
		std::cout << "Initializing server failed! Please see driver log for details.\n";

	/*
	 * Add new tracker with global id.
	 * To update it later we'll need its id in K2Driver,
	 * so we're gonna dump it into an int
	 */

	// Create tracker object with 0 position and orientation, see wiki
	const K2Objects::K2TrackerBase base_tracker;

	/*
	 * If succeed, we're expecting everything more than -1
	 */
	
	std::cout << "Adding new tracker...\n";
	const auto id = k2_api::add_tracker(base_tracker);

	if (id > -1) {
		// Display newly added tracker's id
		std::cout << "Tracker added, id: " << id << '\n';

		/*
		 * Until tracker is added, we can edit its parameters,
		 * let's make it appear at one meter height
		 * and let's give it left foot tracker role hint!
		 * (Same can happen at base tracker declaration, see wiki)
		 */

		// Class conversion if we don't want to use K2PosePacket
		const auto result_pose = k2_api::update_tracker_pose(id,
			K2Objects::K2TrackerPose(glm::quat(), 
				glm::vec3(0.f, 1.f, 0.f))); // y = 1.f: 1 meter height
		
		// Class conversion if we don't want to use K2DataPacket
		const auto result_data = k2_api::update_tracker_data(id,
			K2Objects::K2TrackerData("/devices/KinectToVR/YunYunTrack", // Your funny serial
				"TrackerRole_LeftFoot")); // As described, left foot

		/*
		 * Now, we're going to check operations results
		 * We're expecting return code same as the tracker's id
		 */

		// Check the operation result (pose)
		std::cout << (result_pose == id ? "Tracker pose has been changed!\n" :
			"Tracker pose has not been changed! Please see driver log for details.\n");
		
		// Check the operation result (data)
		std::cout << (result_data == id ? "Tracker data has been changed!\n" :
			"Tracker data has not been changed! Please see driver log for details.\n");

		/*
		 * When everything is set-up then we can finally add our tracker
		 * (Don't forget to check the result!)
		 */

		// Connect tracker (here, one) and expect 'true'
		const auto connect = k2_api::connect_tracker(id);
		if(connect == true)
			std::cout << "Tracker has been connected!\n";
		else
			std::cout << "Tracker has not been connected! Please see driver log for details.\n";
	}
	else 
		std::cout << "Tracker not added! Please see driver log for details.\n";
	
	/*
	 * Now that we added our tracker, we can update its pose
	 * (first activation automatically adds it to devices' list)
	 * After adding the tracker, data cannot be changed
	 */

	// Let's construct new pose for our driver in function call itself, see wiki
	const auto update_pose = k2_api::update_tracker_pose(id,
		K2Objects::K2TrackerPose(
			glm::quat(glm::vec3(0.f, glm::radians(90.f), 0.f)),
			glm::vec3(1.f, 1.f, 0.f)));

	/*
	 * glm::quat was constructed from vec3, that means we're converting eulers to quaternion.
	 * Tracker should be orientated 90 degrees to the side (rotation in y)
	 * and additionally move 1 meter to the side too (was already at 1m height before)
	 */

	// Now, we're going to check the result
	std::cout << (update_pose == id ? "Tracker pose has been changed!\n" :
		"Tracker pose has not been changed! Please see driver log for details.\n");

	/*
	 * At the end, always remember to disconnect.
	 * Otherwise the current process will crash,
	 * although everything will be executed normally.
	 */

	if (k2_api::close_socket() == 0)
		std::cout << "Exit succeed!\n";
	else
		std::cout << "Socket closing failed! Please see driver log for details.\n";
	
	/*
	 * For the rest of commands please see wiki, here I've described the most used ones:
	 * Creation, getting results and post-creation updating pose
	 */
	
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
