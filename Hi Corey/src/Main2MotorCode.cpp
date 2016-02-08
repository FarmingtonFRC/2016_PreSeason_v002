#include "WPILib.h"

class Robot: public SampleRobot
{

	RobotDrive robotDrive; // robot drive system
	Joystick stick; // only joystick
	LiveWindow *lw;
	int autoLoopCounter;
	CANTalon Motor1;
	CANTalon Motor2;
	CANTalon Slave1;
	CANTalon Slave2;
	CANTalon t_motor;
	bool buttonpress = false;
	bool buttonpress2 = false;
	bool launcherState = false;
//	AnalogInput *ourRangefinder;
	bool servoState = false;
	Servo *myServo = new Servo(1);
	Joystick stick2;
	int autoLoopCounter2;
	int directionz;
	int directiony;
	Servo *myServo2 = new Servo(0);
	Servo *myServo3 = new Servo(2);
//	Servo *myServo4 = new Servo(3);

public:
	Robot() :
		robotDrive(Motor1, Motor2),	// these must be initialized in the same order
		stick(5),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),
		Slave1(23),
		Motor1(20),
		Motor2(21),
		t_motor(22),
		stick2(4),
		autoLoopCounter2(0)

	{
		robotDrive.SetExpiration(0.1);
		robotDrive.SetSafetyEnabled(false);
		Slave1.SetControlMode(CANSpeedController::kFollower);
		Slave1.Set(20);
//		CameraServer::GetInstance()->SetQuality(50);
//		CameraServer::GetInstance()->SetSize(2);
//		//the camera name (ex "cam0") can be found through the roborio web interface
//		CameraServer::GetInstance()->StartAutomaticCapture("cam0");

//		t_motor.SetControlMode(CANSpeedController::kPosition);
//		t_motor.SetFeedbackDevice(CANTalon::QuadEncoder);
//		t_motor.SetPosition(10000);
//		ourRangefinder = new AnalogInput(0);

	}
	int winchmax = 5000;
	int winchmin = 0;
	bool servoSetPos(bool s){
		if (s == true){
			myServo->SetAngle(SmartDashboard::GetNumber("servoMin",90));
			myServo2->SetAngle(SmartDashboard::GetNumber("servoMin",90));
			myServo3->SetAngle(SmartDashboard::GetNumber("servoMin",90));
//			myServo4->SetAngle(SmartDashboard::GetNumber("servoMin",90));
		}
		if (s == false){
			myServo->SetAngle(SmartDashboard::GetNumber("servoMax",165));
			myServo2->SetAngle(SmartDashboard::GetNumber("servoMax",165));
			myServo3->SetAngle(SmartDashboard::GetNumber("servoMax",165));
//			myServo4->SetAngle(SmartDashboard::GetNumber("servoMax",165));
		}

		return true;
	}
//	bool motorSetPos(bool z){
/*			if (z == true){
				t_motor.Set(SmartDashboard::GetNumber("winchmax",5000));
			}
			if (z == false){
				t_motor.Set(SmartDashboard::GetNumber("winchmin",0));
			}
*///			return true;
//		}

	bool direction (bool a){
		if (a == true){
			directionz = scaler(stick.GetZ());
			directiony = scaler(stick.GetY());
		}
		if (a == false){
			directionz = -scaler(stick.GetZ());
			directiony = -scaler(stick.GetY());
		}
		return true;

	}

		float scaler(float y){
			if (y>0){
				y=(y*y);
				if (y <=-1){
					y = -1;
			}
				if (y>=1){
					y = 1;
			}
			}
			if (y<0){
				y=-(y*y);
				if (y <=-1){
					y = -1;
					}
				if (y>=1){
					y = 1;
					}
					}
			return y;
		}

	void RobotInit()
	{
//		Auto
		SmartDashboard::PutNumber("Z1",0.5);
		SmartDashboard::PutNumber("Y1",0.0);
		SmartDashboard::PutNumber("Loop",400.0);
		SmartDashboard::PutNumber("Z2",0.0);
		SmartDashboard::PutNumber("Y2",0.0);
		SmartDashboard::PutNumber("Loop2",400);
		SmartDashboard::PutNumber("Winch1",0.0);

//		winch
		SmartDashboard::PutNumber("winchmax",5000);
		SmartDashboard::PutNumber("winchmin",0);

//		Servo
		SmartDashboard::PutNumber("servoMax",165);
		SmartDashboard::PutNumber("servoMin",90);

//		Camera
		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	}
	void Autonomous()
	{
		int Loop = SmartDashboard::GetNumber("Loop",400);
		autoLoopCounter = 0;
		while(autoLoopCounter < Loop && IsEnabled())
		{
			robotDrive.ArcadeDrive(SmartDashboard::GetNumber("Z1",-0.5), SmartDashboard::GetNumber("Y1",0.0));
			autoLoopCounter++;
//			SmartDashboard::PutNumber("Counter",autoLoopCounter);
			Wait(.005);
		}
			robotDrive.ArcadeDrive(SmartDashboard::GetNumber("Z2",0.0), SmartDashboard::GetNumber("Y2",0.0));

			int Loop2 = SmartDashboard::GetNumber("Loop2",400);

		while(autoLoopCounter2 < Loop2 && IsEnabled())
		{
			t_motor.Set(SmartDashboard::GetNumber("Winch1",0.0));
			autoLoopCounter2++;
			Wait(.005);
		}

	}

	void OperatorControl()
		{
			while (IsOperatorControl() && IsEnabled())
			{
				robotDrive.ArcadeDrive(scaler(stick.GetZ()),scaler(stick.GetY()));

//				double volts = ourRangefinder->GetVoltage();
//				SmartDashboard::PutNumber("Voltage",volts);
				//t_motor.Set(stick2.GetZ());
				//t_motor.Set(stick.GetAxis(Joystick::kDefaultThrottleAxis));
				t_motor.Set(stick.GetAxis(Joystick::kThrottleAxis));
//				t_motor.Set(stick.GetAxis(Joystick::Slider));
//				if (stick.GetRawButton(3) == true){
//					t_motor.SetPosition(10000);
//				}
//				if (stick2.GetRawButton(2) == true and buttonpress == false){
//					buttonpress = true;
//					launcherState = !launcherState;
//				}
//				else if (stick2.GetRawButton(2) == false and buttonpress == true){
//					buttonpress = false;// drive with arcade style (use right stick)
//				}

//			if (stick2.GetRawButton(3) == true and buttonpress == false){
//					buttonpress = true;
//					servoState = !servoState;
//									}
//			else if (stick2.GetRawButton(3) == false and buttonpress == true){
//					buttonpress = false;// drive with arcade style (use right stick)
//
//						}
			if (stick.GetRawButton(1) == true){
				servoState = true;
			}
			if (stick.GetRawButton(1) == false){
				servoState = false;
			}
//			if (stick.GetRawButton(2) == true and buttonpress2 == false){
//				buttonpress2 = true;
//				direction = !direction;
//			}
//			if (stick.GetRawButton(2) == false and buttonpress2 == true){
//				buttonpress2 = true;
//
//
//			}
			if (stick.GetRawButton(2) == false){

			}
			servoSetPos(servoState);
//			myServo->Set(.5);
			Wait(0.005);// wait for a motor update time
//			motorSetPos(launcherState);
		}
	}

};

START_ROBOT_CLASS(Robot)
