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
	
	if(ktvr::init_socket(7135) == 0)
		std::cout << "Initializing server succeed!\n";
	else
		std::cout << "Initializing server failed! Please see driver log for details.\n";
	
	/*
	 * Add new tracker with global id.
	 * To update it later we'll need its id in K2Driver,
	 * so we're gonna dump it into an int
	 */

	// Create tracker object with 0 position and orientation, see wiki
	ktvr::K2TrackerBase tracker;
	tracker.data.serial = "/devices/QTrack/WaistTracker";
	tracker.data.role = ktvr::Tracker_Waist;

	/*
	 * If succeed, we're expecting everything more than -1
	 * Also, since isActive is default 0,
	 * tracker won't be spawned until we set it to true.
	 */
	
	std::cout << "Adding new tracker...\n";
	auto response = ktvr::add_tracker(tracker);
	
	// Check for the tracker id and success
	if (tracker.id > -1 && response.success) {
		// Display newly added tracker's id
		std::cout << "Tracker added, id: " << tracker.id << '\n';
		
		/*
		 * Until tracker is added, we can edit its parameters,
		 * let's make it appear at one meter height
		 * and let's give it left foot tracker role
		 * (Same can happen at base tracker declaration, see wiki)
		 */

		// Change the tracker's position, let the orientation be
		tracker.pose.position = glm::vec3(0.f, 1.f, 0.f);

		// Class conversion if we don't want to use K2PosePacket
		const auto result_pose = 
			ktvr::update_tracker_pose(tracker); // y = 1.f: 1 meter height
		
		// Change the tracker's role
		tracker.data.serial = "/devices/JTrack/LFootTracker"; // Your funny serial
		tracker.data.role = ktvr::Tracker_LeftFoot; // As described, left foot

		// Since it wasn't spawned yet, it should spawn now
		tracker.data.isActive = true; // If we change this, tracker should spawn
		
		// Class conversion if we don't want to use K2DataPacket
		const auto result_data = 
			ktvr::update_tracker_data(tracker); // As described, left foot
		
		/*
		 * Actually, you can update both at once,
		 * via calling ktvr::update_tracker(tracker).
		 * But actually, we're going to check those separately.
		 * You may check for success or either, if is the id same as the tracker's one.
		 */
		
		// Check the operation result (pose)
		std::cout << (result_pose.success ? "Tracker pose has been changed!\n" :
			"Tracker pose has not been changed! Please see driver log for details.\n");
		
		// Check the operation result (data)
		std::cout << (result_data.success ? "Tracker data has been changed!\n" :
			"Tracker data has not been changed! Please see driver log for details.\n");
	}
	else {
		std::cout << "Tracker not added! Please see driver log for details.\n";
		
		/*
		* At the end, always remember to disconnect.
		* Otherwise the current process will crash,
		* although everything will be executed normally.
		*/

		if (ktvr::close_socket() == 0)
			std::cout << "Exit succeed!\n";
		else
			std::cout << "Socket closing failed! Please see driver log for details.\n";
		return 0;
	}

	/*
	 * Now that we added our tracker, we can update its pose
	 * (first activation automatically adds it to devices' list)
	 * After adding the tracker, data cannot be changed
	 */

	// Let's construct new pose for our driver in function call itself, see wiki
	// Change the tracker's position, let the orientation be
	tracker.pose.orientation = glm::vec3(0.f, 90.f, 0.f);
	tracker.pose.position = glm::vec3(1.f, 1.f, 0.f);

	// Class conversion if we don't want to use K2PosePacket
	const auto result_pose =
		ktvr::update_tracker_pose(tracker);

	/*
	 * glm::quat was constructed from vec3, that means we're converting eulers to quaternion.
	 * Tracker should be orientated 90 degrees to the side (rotation in y)
	 * and additionally move 1 meter to the side too (was already at 1m height before)
	 */

	// Now, we're going to check the result
	std::cout << (result_pose.success ? "Tracker pose has been changed!\n" :
		"Tracker pose has not been changed! Please see driver log for details.\n");

	/*
	 * At the end, always remember to disconnect.
	 * Otherwise the current process will crash,
	 * although everything will be executed normally.
	 */

	if (ktvr::close_socket() == 0)
		std::cout << "Exit succeed!\n";
	else
		std::cout << "Socket closing failed! Please see driver log for details.\n";
	
	/*
	 * For the rest of commands please see wiki, here I've described the most used ones:
	 * Creation, getting results and post-creation updating pose.
	 * Recently, there was a function added to download already added tracker by id.
	 * (Although only if it was added via K2API, it's not the inputemulator,
	 * it can't overwrite some random device's data!)
	 * Also, if an error occurs, you may check for the result code and it's name inside the enum!
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
