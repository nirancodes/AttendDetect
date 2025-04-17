/*DESCRIPTION OF ATTENDETECT: 
This is a face detection attendance system for clubs, workshops, or lectures that do not have a set attendance list, but have an exact/ estimated number of registrants for these programs that the host will input. 
By determining the attendance based on detected faces, it optimizes the attendance-taking process and determines engagement through the following test cases:
Have all the registrants attended? 
Have none of the registrants attended? 
How many registrants are absent? 
How many extra attendees are present? 
This program also has other features like being able to login and quit the program, 
printing to the frame for detected attendees, recording and saving videos, adding 
timestamps to the file names of these videos for the host to have proof of attendance, 
the intuitive terminal that seeks for user inputs, and engaging sound effects.

*/

#define _CRT_SECURE_NO_WARNINGS
#include<ctime>
#include<opencv2/imgcodecs.hpp> 
#include<opencv2/highgui.hpp> 
#include<opencv2/imgproc.hpp> 
#include<opencv2/objdetect.hpp> 
#include<opencv2/opencv.hpp>
#include<opencv2/videoio.hpp>
#include<windows.h>
#include<iostream>
#include<limits>
#include<string>
#pragma comment(lib, "winmm.lib") /*used to link against the winmm.lib, this is the library that contains the PlaySound function because
previously the linker could not find the definition for the PlaySound function, because then the library containing PlaySound was not linked
*/

using namespace std;
using namespace cv;

//declaring variables soon to be used in main function 
int registrants; //# of people who had registered (not equal to attendees)
string input; //stores response for "cout << "How many registrants are attending today's event?" << endl;"

//STRUCT USED FOR LOGIN 
struct HOST {
	string name; //host's name 
	string user; //host's username 
	string pass; //host's password 
};
//FUNCTION NO. 1: LOGIN SYSTEM: allows host to log in to system w/ a preset username and password 
bool loginSystem() { //return type is boolean b/c login system function returns either a true or false (depending on whether inputted username and password matches w/ preset credentials) 
	struct HOST host; //host is the struct variable to access the members of the struct i.e. name, user & pass 
	cout << " $$$$$$\\    $$\\     $$\\                         $$$$$$$\\             $$\\                           $$\\     " << endl;
	cout << " $$$$$$\\    $$\\     $$\\                         $$$$$$$\\             $$\\                           $$\\  " << endl;
	cout << "$$  __$$\\   $$ |    $$ |                        $$  __$$\\            $$ |                          $$ |   " << endl;
	cout << "$$ /  $$ |$$$$$$\\ $$$$$$\\    $$$$$$\\  $$$$$$$\\  $$ |  $$ | $$$$$$\\ $$$$$$\\    $$$$$$\\   $$$$$$$\\ $$$$$$\\   " << endl;
	cout << "$$$$$$$$ |\\_$$  _|\\_$$  _|  $$  __$$\\ $$  __$$\\ $$ |  $$ |$$  __$$\\_$$  _|  $$  __$$\\ $$  _____|\\_$$  _| " << endl;
	cout << "$$  __$$ |  $$ |    $$ |    $$$$$$$$ |$$ |  $$ |$$ |  $$ |$$$$$$$$ | $$ |    $$$$$$$$ |$$ /        $$ |    " << endl;
	cout << "$$ |  $$ |  $$ |$$\\ $$ |$$\\ $$   ____|$$ |  $$ |$$ |  $$ |$$   ____| $$ |$$\\ $$   ____|$$ |        $$ |$$\\ " << endl;
	cout << "$$ |  $$ |  \\$$$$  |\\$$$$  |\\$$$$$$$\\ $$ |  $$ |$$$$$$$  |\\$$$$$$$\\  \\$$$$  |\\$$$$$$$\\ \\$$$$$$$\\   \\$$$$  |" << endl;
	cout << "\\__|  \\__|   \\____/  \\____/  \\_______|\\__|  \\__|\\_______/  \\_______|  \\____/  \\_______| \\_______|   \\____/ " << endl;
	cout << "Welcome to AttenDetect!\n"; 
	cout << "Description of Program: \n Face detection attendance system for clubs, workshops, lectures that do not have a set attendance list. \n Want to check your engagement and record proof of attendance? Proceed to use the application!"<<endl; 
	//getline functions (ERROR HANDLING: even if the user inputs more than one word/ input w/ spaces, the program does not crash)
	cout << "Please enter host's name: " << endl; //storing name in struct member 'name', making program more user-friendly/ personal 
	getline(cin, host.name); 
	cout << host.name << ", please enter your username: " << endl; //storing username in struct member 'user'
	getline(cin, host.user);
	cout << host.name << ", please enter your password: " << endl; //storing password in struct member 'pass'
	getline(cin, host.pass);
	//IF STATEMENT NO. 1: if the input in members 'user' and 'pass' match with the preset username and password, then the login system function returns true, if not, returns false
	if (host.user == "host123" && host.pass == "event123") {
		return true; 
	}
	else {
		return false; 
	}
}

//FUNCTION NO. 3: FACE DETECTION PROGRAM: detects # of faces, prints on frame, prints to terminal, allows to compare w/ # of registrants, begin/ save recording and write to files...quits/ terminates face detection when explicitly selected
int faceDetectionProgram() {
	VideoCapture video(0); //OOP concept used: NEW CONCEPT: CLASSES - creating instance of VideoCapture class, host can take video from camera, index 0 assigns to my default camera (found via Trial & Error)
	//IF STATEMENT NO.3: error handling: has the camera successfully opened? If not, print an error msg to terminal, indicated by return of -1
	if (!video.isOpened()) { 
		cout << "Camera is unavailable."; 
		return -1; 
	}
	CascadeClassifier facedetect; //OOP concept used: CLASSES - creating instance of CascadeClassifier class to detect faces (uses the trained haarcascade model) 
	Mat vidFrame; //OOP concept used: CLASSES - creating instance of Mat (matrix) class to store the video's frame (aka. 'vidFrame')
	facedetect.load("haarcascade_frontalface_default.xml"); //loading a trained model that detects faces 
	int detectedFaces = 0; //initialized declared int variable that holds the value of the # of detected faces in each frame of captured video to 0 
	int countFrame = 0; //initialized declared int variable that holds the # of frames processed in captured video to 0
	
	int videoWidth = video.get(CAP_PROP_FRAME_WIDTH); //stores value of video frames' width in pixels 
	int videoHeight = video.get(CAP_PROP_FRAME_HEIGHT); //stores value of video frames' height in pixels 
	VideoWriter video_out; //OOP concept used: CLASSES - creating instance of VideoWriter class (used when saving/ writing the captured video to file)
	bool isRecording = false; //initialized declared boolean variable that checks if the program is recording video or not to false, meaning that recording has not begun now

	//LOOP NO. 3: WHILE LOOP: an infinite loop; detects # of faces, prints on frame, prints to terminal, allows to compare w/ # of registrants, begin/ save recording and write to files...quits/ terminates face detection when explicitly selected
	//loop increments; //because a video is a series of images, we need to create an infinite loop, 1 msec = 30 vidFrames, upon combining the images, the video is created 
	//an infinite loop to iterate through all the images 
	while (true) { //NEW CONCEPTS: lines 96 - 107 
		video.read(vidFrame);//reads each frame from the captured video into the frame 
		rotate(vidFrame, vidFrame, ROTATE_180); //frame is rotated because I was experiencing an upside-down video (found via Trial & Error)
		vector<Rect> faces; //Vectors used, OOP concept used: OBJECTS - declaring vector of objects that represent a rectangle (which represents a detected face in the frame)
		
		facedetect.detectMultiScale(vidFrame, faces, 1.3, 5); //uses CascadeClassifier class to detect faces in frame (vidFrame), stored as rectangles in the above faces vector
		//1.3, 5: adhering to the sizing that the trained haarcascade model already uses 
		detectedFaces = faces.size(); //the number of faces in the frame is assigned to detectedFaces (no longer 0), not used until writing to terminal 
		//LOOP NO. 4: FOR LOOP: iterates over each detected face in each frame; values are assigned to detectedFaces
		for (int a = 0; a < faces.size(); a++) {
			rectangle(vidFrame, faces[a].tl(), faces[a].br(), Scalar(50, 50, 255), 5);
			//iterating: red rectangle w/ a thickness of 3 pixels is drawn around each of the detected faces in frame (starting point is top left corner and ending point is bottom right corner), for all detected faces
		}
		//IF STATEMENT NO.4: how many faces were detected in frame? NEW CONCEPT: For each case write the following text overlays (w/ the following font and colour) in the frame
			//test cases: 0 faces, only 1 face, more than 1 faces 
		if (faces.size() == 0) {
			//error handling: alerts host to try again b/c 0 faces were detected 
			putText(vidFrame, to_string(faces.size()) + " faces detected. Please try again.", Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(50, 50, 255), 1);
		}
		else if (faces.size() == 1) {
			putText(vidFrame, to_string(faces.size()) + " face detected!", Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(50, 50, 255), 1);
		}
		else if (faces.size() > 1) {
			putText(vidFrame, to_string(faces.size()) + " faces detected!", Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(50, 50, 255), 1);
		}
		//IF STATEMENT NO.5: //if the video in frame is being recorded, write/ save it, OOP concept used: FILES 
		if (isRecording) {
			video_out.write(vidFrame); 
		}
		imshow("Frame", vidFrame);
		int key = waitKey(1); //waiting for user input
		countFrame++; //loop increments, frame count increases by 1; //because a video is a series of images, we need to create an infinite loop, 1 msec = 30 vidFrames, upon combining the images, the video is created 
	//an infinite loop to iterate through all the images 
	
		//IF STATEMENT NO.6: 
		if (countFrame % 30 == 0) { //following choices are presented for every 30 frames of the video
			cout << "\nPress 'c' to check members \n Press 'r' to begin/ end recording \n Press 'q' to quit video capture and log out\n";
			string command; //whatever input the user enters is stored as a string, assigned to this variable (must be either 'c', 'r', 'q' - if another letter/ word/ number is inputted, it is considered invalid, but user gets to input again after 30 frames)
			getline(cin, command); //accomodates words/ inputs w/ spaces as flawed input as well 
			//IF STATEMENT NO.7: 
			if (command == "c") { //if host checks members, first the number of attendees (detected faces) is written to terminal 
				cout << "Number of attendees (registrants present): " << detectedFaces << endl; 
				//test cases: 
				//IF STATEMENT NO.8: are registrants greater than attendees? 
				if (registrants > detectedFaces) {
					int absences = registrants - detectedFaces; //absences are the registrants who have not attended/ been detected
					cout << "Number of registrants absent: " << absences<<endl;
					//IF STATEMENT NO.9: are absences equal to registrants, meaning no one joined yet?
					if (absences == registrants) {
						cout << "No registrants have joined yet :(";
					}
				}
				else if (detectedFaces > registrants) { //are there more attendees than # of registrants? those are extra members.
					int extraMembers = detectedFaces - registrants;
					cout << "Number of extra members: " << extraMembers;
				}
				else if (detectedFaces = registrants) { //have all registrants attended? 
					cout << "Success! All " << registrants << " registrants are present!";
					PlaySound(TEXT("success.wav"), NULL, SND_FILENAME | SND_ASYNC); //OOP concept used: FILES; NEW CONCEPT: wav sound file is played w/ PlaySound indicating full attendance, a Windows API function
				}
			}
			else if (command == "r") { //if host presses r, they are either asking to end recording (if recording had already begun), or they want to start recording (if recording had not begun)
				if (isRecording) { //IF STATEMENT NO.10: if video was already recording, by now pressing 'r', host wants to stop recording
					video_out.release(); //recording stops and saves, releasing the aforementioned VideoWriter object 
					cout << "Recording ended and saved." << endl;
					PlaySound(TEXT("chime.wav"), NULL, SND_FILENAME | SND_ASYNC);  //OOP concept used: FILES; NEW CONCEPT: wav sound file is played w/ PlaySound indicating recording beginning/ ending, a Windows API function
				}
				else { //if program is not recording, once saved, its file name will be written with a timestamp: 
					time_t now = time(0); //current time 
					tm*ltm = localtime(&now); //converted to local time 
					string vidName = "attendanceRecording_Date" //vidName stores the video's name as the following:
						//to_string converts yr, mth, day, hr, min, all are concatenated 
						+ to_string(1900 + ltm->tm_year)
						+"-"
						+ to_string(1 + ltm->tm_mon)
						+ "-"
						+ to_string(ltm->tm_mday)
						+ "_Time"
						+ to_string(ltm->tm_hour)
						+ ","
						+ to_string(ltm->tm_min)
						+ ".avi"; //saved as .avi, Trial and Error: only permissible format
					video_out.open(vidName, VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(videoWidth, videoHeight)); 
					cout << "Recording has begun." << endl; 
					//four character code used to compress frames (motion-jpeg), 10 frames per second in video, storing in aforementioned height and width 
					PlaySound(TEXT("chime.wav"), NULL, SND_FILENAME | SND_ASYNC);
					//OOP concept used: FILES; NEW CONCEPT: wav sound file is played w/ PlaySound indicating recording beginning/ ending, a Windows API function
				}
				isRecording = !isRecording; 
				//a video being recorded is stopped and a video that wasn't being recorded is now being recorded, upon pressing 'r' 
			}
			else if (command == "q") { //when host wants to quit the face detection attendance: 
				if (isRecording) { //if there was a recording in progress, but host forgets to end recording, it is ended, saved and written to a file by the program 
					video_out.release();
					cout << "We have ended and saved your video." << endl;
				}
				cout << "Thanks for using 'AttenDetect'. Goodbye!"<<endl; 
				break; //quitting the face detection program means host breaks out of the infinite loop
			}
			else{
				cout << "Invalid Input. Please try next time."; 
			}
		}
	}
	return 0;  //indicating a successful run of program 
}

//FUNCTION NO. 2: MAIN FUNCTION (allows user to successfully log in and add a # of registrants) 
	int main() {
		//initializing a boolean variable to false to check if the login attempt was successful 
		bool logInSuccess = false;
		//LOOP NO. 1: DO...WHILE (ERROR HANDLING: allowing the user more than 1 try during flawed inputs) 
		do {
			logInSuccess = loginSystem();
			//this variable's value changes based on whether the loginSystem function returned true (successful attempt) or false
			if (!logInSuccess) { //IF STATEMENT NO. 2: if this variable's value is false, then an error message shows
				cout << "Incorrect Username or Password credentials. Please try again!" << endl;
			}
		} while (!logInSuccess); //as long as these attempts are unsuccessful, the host can continue to retry to login 
		cout << "Successful login!" << endl;
		//when the user breaks out of this loop (meaning their login credentials match the preset ones), they have successfully logged in to use the application

		//LOOP NO. 2: DO...WHILE (ERROR HANDLING: allowing the user more than 1 try during flawed inputs) 
		//OOP concept used: EXCEPTION HANDLING & OBJECTS 
		do {
			cout << "How many registrants are attending today's event?" << endl;
			getline(cin, input); //# of registrants is stored in string variable 'input'; takes in spaces too (no errors w/ spaces)
			try { //NEW CONCEPT: try block & exceptions -- code is executed and thrown exceptions (which disrupt the program's flow) are caught by catch block
				//WHY DID I USE THIS INSTEAD OF cin.clear()? 
				/*
				with cin.clear() & cin.ignore(), although it was handling negative integers, I found that it was not handling text inputs as errors and that my program was crashing; cin was entering a fail state
				why was it entering a fail state: ex. if I entered "10 attendees", it took "10", but also accepted the word in the input stream, causing disruptions
				by converting the string value in 'input' and then converting it to an int via stoi function, if the conversion fails, depending on whether it doesn't represent a positive integer/ number greater than 0, both cases (words and negative numbers) are properly handled (explained below)
				*/ 
				registrants = stoi(input); //stoi function converts string input (ex. '4') to integer (ex. 4) ; integer value assigned to registrants
				//stoi function throws the invalid argument exception object, if this string input has digitless chars/ cannot be converted into an int 
				if (registrants < 0) throw invalid_argument("Negative number");
				//if the int value assigned to registrants is a negative number, stoi function again throws invalid argument exception object w/ a msg
				cout << "Thanks! Video attendance will now begin."<<endl;
				//but if no exceptions were thrown by stoi, and if the host inputs 0/ a positive number (desired input), then video attendance starts 
			}
			catch (invalid_argument&) { //upon catching either of the invalid argument exceptions, this line print out an error msg 
				cout << "Invalid user input. Please enter a positive number of registrants." << endl;
				continue; //used because the program was previously crashing; so found that this skips the present loop iteration and immediately goes back to the start of the loop for the next iteration (giving users another chance during invalid inputs) (found via Trial & Error)
			}
		} while (registrants < 0); //what happens in 'do' only occurs when the int value assigned to 'registrants' through stoi is less than 0 (flawed inputs)
			


		faceDetectionProgram(); //call for faceDetectionProgram to run after successful login & valid input for registrants 
		return 0; //indicating a successful run of program 
	}
