#include "WPILib.h"
#include <Timer.h>

const int buttonStart = 8;//Start
const int buttonA = 1; //A
const int triggerR = 3;//R Trigger
const int triggerL = 2; //L Trigger
const int buttonX = 3; //X
const int thumbpadL_Y = 1; //L Y Axis
const int thumbpadR_Y = 5; //R Y Axis
const int bumperR = 6; //R Bumper
const int bumperL = 5; //L Bumper
const int buttonBack = 7; //Back


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
//	int directionz;
//	int directiony;
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
	int toggleIntakeWasPressed = 0;
	int intakeMode = 0;
	float scalerValue = 1;
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
		Motor2.SetInverted(true); //12
		Slave2.SetInverted(true);//14
		arm_Motor.SetInverted(false);//23
		t_motor.SetInverted(true);//23
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
void setScalerValue(){
	scalerValue = stick.GetRawAxis(3);
	scalerValue += 2;

}
		float scaler(float y){
			if (y>0){
				y= pow(fabs(y), scalerValue);
				if (y>=1){y = 1;}
			}
			if (y<0){
				y=-pow(fabs(y), scalerValue);
				if (y <=-1){y = -1;}
			}
			return y;
//			SmartDashboard::GetNumber("scalerValue",scalerValue);

		}
		void manualShooter(){

			if (stick2.GetRawButton(buttonStart) == true and manShootWasPressed == 0){
				manShootWasPressed = 1;
				if (manShootMode == 1){
					manShootMode = 0;
				}
				else if (manShootMode == 0){
					manShootMode = 1;
				}
			}
			if (stick2.GetRawButton(buttonStart) == false and manShootWasPressed == 1){
				manShootWasPressed = 0;
			}
		SmartDashboard::PutNumber("manShootMode",manShootMode);

		}
		void toggleIntake(){
			if (stick2.GetRawButton(bumperR) == true){
					intakeMode = 1;}

			if (stick2.GetRawButton(bumperR) == false and stick.GetRawButton(bumperL) == false){
					intakeMode = 0;
				}
			if (stick2.GetRawButton(bumperL) == true){
					intakeMode = -1;
				}
			}

		void toggleIntakeMode(){
			if (intakeMode == 1){
				intake_Spin_Motor.Set(1);
			}
			if (intakeMode == 0){
				intake_Spin_Motor.Set(0);
			}
			if (intakeMode == -1){
				intake_Spin_Motor.Set(-1);
						}

		}
		void updateShooter(){
			SmartDashboard::PutBoolean("stateDisarmed", stateDisarmed);
			SmartDashboard::PutBoolean("stateArming1", stateArming1);
			SmartDashboard::PutBoolean("stateArming2", stateArming2);
			SmartDashboard::PutBoolean("stateArmed", stateArmed);
			SmartDashboard::PutBoolean("stateFiring1", stateFiring1);
			float winchLocked = SmartDashboard::GetNumber("winchLocked",-4500.00);
//			float winchUnlocked = SmartDashboard::GetNumber("winchUnlocked",-3000.00);
			float winchMidway = SmartDashboard::GetNumber("winchMidway",-0.00);
//			float winchServoTrigger = SmartDashboard::GetNumber("winchServoTrigger",-0.00);
//			float winchOverUnlocked = SmartDashboard::GetNumber("winchOverUnlocked",0.00);
			float servoLocked = SmartDashboard::GetNumber("servoLocked",5.00);
			float servoUnlocked = SmartDashboard::GetNumber("servoUnlocked",175.00);
			float motorStopped = 0.00;
			float motorWinding = SmartDashboard::GetNumber("motorWinding",-.70);
			float motorUnwinding = SmartDashboard::GetNumber("motorUnwinding",.50);
//			float motorFastUnwinding = SmartDashboard::GetNumber("motorFastUnwinding",.80);

			if (stateDisarmed == true){
//				t_motor.Set(0);
			}
			if (stick2.GetRawButton(buttonA) == true and stateDisarmed == true){
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
				if (stick2.GetRawAxis(triggerR) > 0){
					t_motor.Set(scaler(-1*(stick2.GetRawAxis(triggerR))));
				}
				else if (stick2.GetRawAxis(triggerL) > 0){
					t_motor.Set(scaler(stick2.GetRawAxis(triggerL)));
				}
				else if (stick2.GetRawAxis(triggerL) == 0 and stick2.GetRawAxis(triggerR) == 0){
					t_motor.Set(0);
				}

				if (stick2.GetRawButton(buttonX) == true){
					myServo->SetAngle(175);
				}
				if (stick2.GetRawButton(buttonX) == false){
					myServo->SetAngle(5);
				}
			}
		}
	void RobotInit()
	{
//		Auto
		SmartDashboard::PutNumber("ForwardSpeed",0.5);
		SmartDashboard::PutNumber("ForwardTime",2.0);
		SmartDashboard::PutNumber("TwistSpeed",0.5);
		SmartDashboard::PutNumber("TwistTime",2.0);

// State function stuff
		SmartDashboard::PutNumber("winchLocked",-4500.00);
//		SmartDashboard::PutNumber("winchUnlocked",-3000.00);
		SmartDashboard::PutNumber("winchMidway",-0.00);
		SmartDashboard::PutNumber("winchServoTrigger",-0);
		SmartDashboard::PutNumber("winchOverUnlocked",0.00);
		SmartDashboard::PutNumber("servoLocked",5.00);
		SmartDashboard::PutNumber("servoUnlocked",175.00);
		SmartDashboard::PutNumber("motorWinding",-.70);
		SmartDashboard::PutNumber("motorUnwinding",.50);
		SmartDashboard::PutNumber("motorFastUnwinding",.80);
//		SmartDashboard::PutNumber("scalerValue",scalerValue);

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
		double forwardTime = SmartDashboard::GetNumber("ForwardTime",2.0);
		double forwardSpeed = SmartDashboard::GetNumber("ForwardSpeed",0.5);
		double twistTime = SmartDashboard::GetNumber("TwistTime",2.0);
		double twistSpeed = SmartDashboard::GetNumber("TwistSpeed",0.5);

				Timer *timer = new Timer();
				timer->Start();
		//		int Loop = SmartDashboard::GetNumber("ForwardTime",400);
		//				autoLoopCounter = 0;
						while(timer->Get() < forwardTime && IsEnabled())
						{
							robotDrive.ArcadeDrive(0.0, forwardSpeed);
		//					autoLoopCounter++;
				//			SmartDashboard::PutNumber("Counter",autoLoopCounter);
							Wait(.005);
						}

						timer->Reset();

		//				int Loop2 = SmartDashboard::GetNumber("TwistTime",400);
		//				autoLoopCounter2 = 0;
						while(timer->Get() < twistTime && IsEnabled())
						{
							robotDrive.ArcadeDrive(twistSpeed, 0.0);
							autoLoopCounter2++;
							Wait(.005);
						}

						timer->Stop();

						robotDrive.ArcadeDrive(0.0,0.0);

						stateDisarmed = false;
						stateArming1 = false;
						stateArming2 = false;
						stateArmed = true;
						stateFiring1 = false;
						stateFiring2 = false;

						free(timer);

	}

	void OperatorControl()
		{
			while (IsOperatorControl() && IsEnabled())
			{
				robotDrive.ArcadeDrive(scaler(stick.GetZ()),scaler(stick.GetY()));
				SmartDashboard::PutNumber("StickZ",stick.GetZ());
				SmartDashboard::PutNumber("StickZscaled",scaler(stick.GetZ()));


				finger_Motor.Set(scaler(stick2.GetRawAxis(thumbpadL_Y)));
//				scalerValue = stick.GetRawAxis(3);
				arm_Motor.Set(scaler(stick2.GetRawAxis(thumbpadR_Y)));
				manualShooter();
				shootingModes();
				toggleIntake();
				toggleIntakeMode();
				setScalerValue();
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

			if (stick2.GetRawButton(buttonBack) == true){
				stateDisarmed = true;
				stateArming1 = false;
				stateArming2 = false;
				stateArmed = false;
				stateFiring1 = false;
				stateFiring2 = false;
				t_motor.SetPosition(0);

			}
			toggleIntake();
//			if (stick2.GetRawButton(5) == true){
//				intake_Spin_Motor.Set(1);
//			}
//			if (stick2.GetRawButton(5) == false){
//				intake_Spin_Motor.Set(0);
//			}
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
