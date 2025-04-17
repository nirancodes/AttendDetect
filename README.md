# AttendDetect 🎥🧑‍💼

A real-time, face-detection-based attendance system for virtual or hybrid workplaces. Detects presence, absence, unauthorized attendees, and automatically logs and records session data — no manual check-ins needed.

> Built with OpenCV and C++ | Audio feedback using Windows API | No third-party backend needed

---

### 📌 Features

- ✅ Real-time **facial recognition** attendance with visual & terminal-based logging
- 🔐 Secure **login system** for host authentication
- 📊 Detects:  
  - All registrants present  
  - No one present  
  - Extra participants (security check)  
  - Partial attendance / absences  
- 🎞️ Saves recordings with timestamped filenames (e.g., `attendanceRecording_Date2025-04-17_Time15,23.avi`)
- 🔊 Windows API **sound alerts** for success and recording states
- 🧪 **Test logic** integrated into the runtime to handle real-world edge cases

---

### 🚀 Getting Started

#### Prerequisites

- Windows machine with:
  - C++ Compiler (Visual Studio recommended)
  - [OpenCV installed and configured](https://docs.opencv.org/)
  - Webcam
- Haar Cascade XML file in root folder (`haarcascade_frontalface_default.xml`)
- `success.wav` and `chime.wav` in the `/assets` folder

#### 🛠 Build & Run (Visual Studio)

1. Clone the repository:
   ```bash
   git clone https://github.com/nirancodes/AttendDetect.git
   cd AttendDetect

#### For a Quick Test: 
Enter login credentials:
Username: host123
Password: event123
Set the number of employees (e.g., 4)
Turn your webcam on and try the following:
✅ Sitting alone (detects 1 person)
✅ Bringing others into view (detects multiple)
✅ Press r to start or stop video recording
✅ Press q to quit the session
