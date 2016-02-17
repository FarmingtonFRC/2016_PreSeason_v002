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
	CANTalon arm_Motor;
	CANTalon finger_Motor;
	CANTalon intake_Spin_Motor;
	CANTalon intake_Winch_Motor;
	bool buttonpress = false;
	bool buttonpress2 = false;
	bool launcherState = false;
//	AnalogInput *ourRangefinder;
	bool servoState = false;
	Servo *myServo = new Servo(1);
	Joystick stick2;
//	USBCamera *targetCam = new USBCamera("cam0",1);
	int autoLoopCounter2;
	int directionz;
	int directiony;
	Servo *myServo2 = new Servo(0);
	Servo *myServo3 = new Servo(2);
//	Servo *myServo4 = new Servo(3);

//	State function declares
	bool stateDisarmed = true;
	bool stateArming1 = false;
	bool stateArming2 = false;
	bool stateArmed = false;
	bool stateFiring1 = false;
	bool stateFiring2 = false;
	int manShootWasPressed = 0;
	int manShootMode = 0;

public:
	Robot() :
		robotDrive(Motor1, Motor2),	// these must be initialized in the same order
		stick(5),		// as they are declared above.
		lw(LiveWindow::GetInstance()),
		autoLoopCounter(0),
		Motor1(21),
		Motor2(12),
		Slave1(20),
		Slave2(14),
		t_motor(13),
		arm_Motor(23),
		finger_Motor(22),
		intake_Spin_Motor(11),
		intake_Winch_Motor(13),
		stick2(4),
		autoLoopCounter2(0)
	{
		robotDrive.SetExpiration(0.1);
		robotDrive.SetSafetyEnabled(false);
		Slave1.SetControlMode(CANSpeedController::kFollower);
		Slave1.Set(21);
		Slave2.SetControlMode(CANSpeedController::kFollower);
		Slave2.Set(12);
//		t_motor.SetControlMode(CANSpeedController::kVoltage);
//		t_motor.Set(0);
//		CameraServer::GetInstance()->SetQuality(50);

//		CameraServer::GetInstance()->SetSize(2);
//		//the camera name (ex "cam0") can be found through the roborio web interface
//		CameraServer::GetInstance()->StartAutomaticCapture("cam0");

		t_motor.SetControlMode(CANSpeedController::kPercentVbus);
//		t_motor.SetVoltageCompensationRampRate(24.0);
		t_motor.SetFeedbackDevice(CANTalon::QuadEncoder);
		t_motor.SetPosition(0);
//		t_motor.SetPID(1, 0, 0);
		arm_Motor.SetControlMode(CANSpeedController::kPercentVbus);
		finger_Motor.SetControlMode(CANSpeedController::kPercentVbus);
//		ourRangefinder = new AnalogInput(0);

	}
	int winchmax = 5000;
	int winchmin = 0;
	bool servoSetPos(bool s){
		if (s == true){
			myServo->SetAngle(SmartDashboard::GetNumber("servoMin",5));
			myServo2->SetAngle(SmartDashboard::GetNumber("servoMin",5));
			myServo3->SetAngle(SmartDashboard::GetNumber("servoMin",5));
//			myServo4->SetAngle(SmartDashboard::GetNumber("servoMin",5));
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
//			if (z == true){
//				t_motor.Set(SmartDashboard::GetNumber("winchmax",5000));
//			}
//			if (z == false){
//				t_motor.Set(SmartDashboard::GetNumber("winchmin",0));
//			}
//			return true;
//		}

//	bool direction (bool a){
//		if (a == true){
//			directionz = scaler(stick.GetZ());
//			directiony = scaler(stick.GetY());
//		}
//		if (a == false){
//			directionz = -scaler(stick.GetZ());
//			directiony = -scaler(stick.GetY());
//		}
//		return true;
//
//	}

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
		void manualShooter(){

			if (stick2.GetRawButton(7) == true and manShootWasPressed == 0){
				manShootWasPressed = 1;
				if (manShootMode == 1){
					manShootMode = 0;
				}
				else if (manShootMode == 0){
					manShootMode = 1;
				}
			}
			if (stick2.GetRawButton(7) == false and manShootWasPressed == 1){
				manShootWasPressed = 0;
			}
		SmartDashboard::PutNumber("manShootMode",manShootMode);

		}

		void updateShooter(){
			SmartDashboard::PutBoolean("stateDisarmed", stateDisarmed);
			SmartDashboard::PutBoolean("stateArming1", stateArming1);
			SmartDashboard::PutBoolean("stateArming2", stateArming2);
			SmartDashboard::PutBoolean("stateArmed", stateArmed);
			SmartDashboard::PutBoolean("stateFiring1", stateFiring1);
			float winchLocked = SmartDashboard::GetNumber("winchLocked",-4100.00);
//			float winchUnlocked = SmartDashboard::GetNumber("winchUnlocked",-3000.00);
			float winchMidway = SmartDashboard::GetNumber("winchMidway",-0.00);
			float winchServoTrigger = SmartDashboard::GetNumber("winchServoTrigger",-0.00);
			float winchOverUnlocked = SmartDashboard::GetNumber("winchOverUnlocked",0.00);
			float servoLocked = SmartDashboard::GetNumber("servoLocked",5.00);
			float servoUnlocked = SmartDashboard::GetNumber("servoUnlocked",175.00);
			float motorStopped = 0.00;
			float motorWinding = SmartDashboard::GetNumber("motorWinding",-.70);
			float motorUnwinding = SmartDashboard::GetNumber("motorUnwinding",.50);
			float motorFastUnwinding = SmartDashboard::GetNumber("motorFastUnwinding",.80);

			if (stateDisarmed == true){
//				t_motor.Set(0);
			}
			if (stick2.GetRawButton(1) == true and stateDisarmed == true){
				myServo->SetAngle(servoLocked);
				stateDisarmed = false;
				stateArming1 = true;
			}
			if (stateArming1 == true){
				t_motor.Set(motorWinding);
				myServo->SetAngle(servoLocked);
				if (t_motor.GetEncPosition() < winchLocked){
					t_motor.Set(motorStopped);
					stateArming1 = false;
					stateArming2 = true;
				}
			}
			if (stateArming2 == true){
				t_motor.Set(motorUnwinding);
				if (t_motor.GetEncPosition() >= winchMidway){
					t_motor.Set(motorStopped);
					stateArming2 = false;
					stateArmed = true;
				}
			}
			if (stick.GetRawButton(1) == true and stateArmed == true){
				stateArmed = false;
				stateFiring1 = true;
			}
			if (stateFiring1 == true){
				t_motor.Set(0);
				myServo->SetAngle(servoUnlocked);
				stateFiring1 = false;
				stateDisarmed = true;
			}
		}
		void shootingModes(){
			if (manShootMode == 0){
				updateShooter();
			}
			if (manShootMode == 1){
				t_motor.Set(scaler(stick2.GetRawAxis(3)));
				if (stick2.GetRawButton(3) == true){
					myServo->SetAngle(175);
				}
				if (stick2.GetRawButton(3) == false){
					myServo->SetAngle(5);
				}
			}
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

// State function stuff
		SmartDashboard::PutNumber("winchLocked",-4100.00);
//		SmartDashboard::PutNumber("winchUnlocked",-3000.00);
		SmartDashboard::PutNumber("winchMidway",-0.00);
		SmartDashboard::PutNumber("winchServoTrigger",-0);
		SmartDashboard::PutNumber("winchOverUnlocked",0.00);
		SmartDashboard::PutNumber("servoLocked",5.00);
		SmartDashboard::PutNumber("servoUnlocked",175.00);
		SmartDashboard::PutNumber("motorWinding",-.70);
		SmartDashboard::PutNumber("motorUnwinding",.50);
		SmartDashboard::PutNumber("motorFastUnwinding",.80);

//		Servo
//		SmartDashboard::PutNumber("servoMax",165);
//		SmartDashboard::PutNumber("servoMin",90);

//		Camera
//		CameraServer::GetInstance()->SetQuality(50);
//		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
//		targetCam->StartCapture();

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
				if (stick2.GetPOV(0) == 0){
					finger_Motor.Set(.75);
				}
				else if (stick2.GetPOV(0) == 180){
					finger_Motor.Set(-.75);
				}
				else if (stick2.GetPOV(0) == -1){
					finger_Motor.Set(0);
				}
				SmartDashboard::PutNumber("POV",stick2.GetPOV(0));

				arm_Motor.Set(stick2.GetRawAxis(1));
				manualShooter();
				shootingModes();
//				double volts = ourRangefinder->GetVoltage();
//				SmartDashboard::PutNumber("Voltage",volts);
				//t_motor.Set(stick2.GetZ());
				//t_motor.Set(stick.GetAxis(Joystick::kDefaultThrottleAxis));
//				t_motor.Set(stick.GetAxis(Joystick::kThrottleAxis));
//				finger_Motor.Set(stick2.GetAxis(Joystick::kThrottleAxis));
//				arm_Motor.Set(stick2.GetY());

		//		Current Control mode Debug
				SmartDashboard::PutNumber("Motor30 Current",t_motor.GetOutputCurrent());
				SmartDashboard::PutNumber("Position",t_motor.GetPosition());
//				t_motor.Set(stick.GetAxis(Joystick::Slider));
//				if (stick.GetRawButton(3) == true){
//					t_motor.SetPosition(10000);
//				}

//			if (stick2.GetRawButton(7) == true and buttonpress == false){
//

//									}
//			else if (stick2.GetRawButton(3) == false and buttonpress == true){
//					buttonpress = false;// drive with arcade style (use right stick)
//
//						}
			SmartDashboard::PutBoolean("buttonpress state",buttonpress);

			if (stick2.GetRawButton(3) == true){
				stateDisarmed = true;
				stateArming1 = false;
				stateArming2 = false;
				stateArmed = false;
				stateFiring1 = false;
				stateFiring2 = false;
				t_motor.SetPosition(0);

			}
			if (stick2.GetRawButton(5) == true){
				intake_Spin_Motor.Set(1);
			}
			if (stick2.GetRawButton(5) == false){
				intake_Spin_Motor.Set(0);
			}
			if (stick.GetRawButton(2) == true and buttonpress2 == false){
				myServo->SetAngle(175);
			}
			if (stick.GetRawButton(2) == false and buttonpress2 == true){
				myServo->SetAngle(5);


			}
//			servoSetPos(servoState);
//			myServo->Set(.5);
			Wait(0.005);// wait for a motor update time
//			motorSetPos(launcherState);
		}
	}

};

START_ROBOT_CLASS(Robot)
