################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AStar.cpp \
../src/AbstractActuator.cpp \
../src/AbstractAgent.cpp \
../src/AbstractSensor.cpp \
../src/BoundedVector.cpp \
../src/CommunicationService.cpp \
../src/CompassSensor.cpp \
../src/FileTraceFunction.cpp \
../src/Goal.cpp \
../src/GoalShape.cpp \
../src/KalmanFilter.cpp \
../src/LaserDistanceSensor.cpp \
../src/LidarDistanceSensor.cpp \
../src/LineShape.cpp \
../src/LogTextCtrl.cpp \
../src/Logger.cpp \
../src/Main.cpp \
../src/MainApplication.cpp \
../src/MainFrameWindow.cpp \
../src/MathUtils.cpp \
../src/ModelObject.cpp \
../src/NotificationHandler.cpp \
../src/Notifier.cpp \
../src/ObjectId.cpp \
../src/Observer.cpp \
../src/OdometerSensor.cpp \
../src/Particle.cpp \
../src/ParticleFilter.cpp \
../src/RectangleShape.cpp \
../src/Robot.cpp \
../src/RobotShape.cpp \
../src/RobotWorld.cpp \
../src/RobotWorldCanvas.cpp \
../src/Server.cpp \
../src/Shape2DUtils.cpp \
../src/StdOutTraceFunction.cpp \
../src/SteeringActuator.cpp \
../src/Trace.cpp \
../src/ViewObject.cpp \
../src/Wall.cpp \
../src/WallShape.cpp \
../src/WayPoint.cpp \
../src/WayPointShape.cpp \
../src/WidgetTraceFunction.cpp \
../src/Widgets.cpp 

CPP_DEPS += \
./src/AStar.d \
./src/AbstractActuator.d \
./src/AbstractAgent.d \
./src/AbstractSensor.d \
./src/BoundedVector.d \
./src/CommunicationService.d \
./src/CompassSensor.d \
./src/FileTraceFunction.d \
./src/Goal.d \
./src/GoalShape.d \
./src/KalmanFilter.d \
./src/LaserDistanceSensor.d \
./src/LidarDistanceSensor.d \
./src/LineShape.d \
./src/LogTextCtrl.d \
./src/Logger.d \
./src/Main.d \
./src/MainApplication.d \
./src/MainFrameWindow.d \
./src/MathUtils.d \
./src/ModelObject.d \
./src/NotificationHandler.d \
./src/Notifier.d \
./src/ObjectId.d \
./src/Observer.d \
./src/OdometerSensor.d \
./src/Particle.d \
./src/ParticleFilter.d \
./src/RectangleShape.d \
./src/Robot.d \
./src/RobotShape.d \
./src/RobotWorld.d \
./src/RobotWorldCanvas.d \
./src/Server.d \
./src/Shape2DUtils.d \
./src/StdOutTraceFunction.d \
./src/SteeringActuator.d \
./src/Trace.d \
./src/ViewObject.d \
./src/Wall.d \
./src/WallShape.d \
./src/WayPoint.d \
./src/WayPointShape.d \
./src/WidgetTraceFunction.d \
./src/Widgets.d 

OBJS += \
./src/AStar.o \
./src/AbstractActuator.o \
./src/AbstractAgent.o \
./src/AbstractSensor.o \
./src/BoundedVector.o \
./src/CommunicationService.o \
./src/CompassSensor.o \
./src/FileTraceFunction.o \
./src/Goal.o \
./src/GoalShape.o \
./src/KalmanFilter.o \
./src/LaserDistanceSensor.o \
./src/LidarDistanceSensor.o \
./src/LineShape.o \
./src/LogTextCtrl.o \
./src/Logger.o \
./src/Main.o \
./src/MainApplication.o \
./src/MainFrameWindow.o \
./src/MathUtils.o \
./src/ModelObject.o \
./src/NotificationHandler.o \
./src/Notifier.o \
./src/ObjectId.o \
./src/Observer.o \
./src/OdometerSensor.o \
./src/Particle.o \
./src/ParticleFilter.o \
./src/RectangleShape.o \
./src/Robot.o \
./src/RobotShape.o \
./src/RobotWorld.o \
./src/RobotWorldCanvas.o \
./src/Server.o \
./src/Shape2DUtils.o \
./src/StdOutTraceFunction.o \
./src/SteeringActuator.o \
./src/Trace.o \
./src/ViewObject.o \
./src/Wall.o \
./src/WallShape.o \
./src/WayPoint.o \
./src/WayPointShape.o \
./src/WidgetTraceFunction.o \
./src/Widgets.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DWXMSW -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D"`wx-config --cxxflags --libs`" -I/usr/include/c++/ -I/usr/local/include/wx-3.2/ -I/usr/include/boost/ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/AStar.d ./src/AStar.o ./src/AbstractActuator.d ./src/AbstractActuator.o ./src/AbstractAgent.d ./src/AbstractAgent.o ./src/AbstractSensor.d ./src/AbstractSensor.o ./src/BoundedVector.d ./src/BoundedVector.o ./src/CommunicationService.d ./src/CommunicationService.o ./src/CompassSensor.d ./src/CompassSensor.o ./src/FileTraceFunction.d ./src/FileTraceFunction.o ./src/Goal.d ./src/Goal.o ./src/GoalShape.d ./src/GoalShape.o ./src/KalmanFilter.d ./src/KalmanFilter.o ./src/LaserDistanceSensor.d ./src/LaserDistanceSensor.o ./src/LidarDistanceSensor.d ./src/LidarDistanceSensor.o ./src/LineShape.d ./src/LineShape.o ./src/LogTextCtrl.d ./src/LogTextCtrl.o ./src/Logger.d ./src/Logger.o ./src/Main.d ./src/Main.o ./src/MainApplication.d ./src/MainApplication.o ./src/MainFrameWindow.d ./src/MainFrameWindow.o ./src/MathUtils.d ./src/MathUtils.o ./src/ModelObject.d ./src/ModelObject.o ./src/NotificationHandler.d ./src/NotificationHandler.o ./src/Notifier.d ./src/Notifier.o ./src/ObjectId.d ./src/ObjectId.o ./src/Observer.d ./src/Observer.o ./src/OdometerSensor.d ./src/OdometerSensor.o ./src/Particle.d ./src/Particle.o ./src/ParticleFilter.d ./src/ParticleFilter.o ./src/RectangleShape.d ./src/RectangleShape.o ./src/Robot.d ./src/Robot.o ./src/RobotShape.d ./src/RobotShape.o ./src/RobotWorld.d ./src/RobotWorld.o ./src/RobotWorldCanvas.d ./src/RobotWorldCanvas.o ./src/Server.d ./src/Server.o ./src/Shape2DUtils.d ./src/Shape2DUtils.o ./src/StdOutTraceFunction.d ./src/StdOutTraceFunction.o ./src/SteeringActuator.d ./src/SteeringActuator.o ./src/Trace.d ./src/Trace.o ./src/ViewObject.d ./src/ViewObject.o ./src/Wall.d ./src/Wall.o ./src/WallShape.d ./src/WallShape.o ./src/WayPoint.d ./src/WayPoint.o ./src/WayPointShape.d ./src/WayPointShape.o ./src/WidgetTraceFunction.d ./src/WidgetTraceFunction.o ./src/Widgets.d ./src/Widgets.o

.PHONY: clean-src

