/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <base/samples/IMUSensors.hpp>
#include <boost/math/constants/constants.hpp>
//#include <base/Pose.hpp>

using namespace north_seeker;

const double pi = boost::math::constants::pi<double>();

Task::Task(std::string const& name)
    : TaskBase(name), x(0.0), y(0.0), z(0.0),counter(0)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine)
{
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;
    sampling_period = base::Time::fromSeconds(_sampling_period.get());
    std::cout << "Sampling for " << sampling_period << " seconds" << std::endl;
    return true;
}
bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;
    start = base::Time::now();
    std::cout << "DON'T MOVE - Searching True North" << std::endl;
    return true;
}

void Task::updateHook()
{
    TaskBase::updateHook();
    base::samples::IMUSensors sensors;
    _imusamples.read(sensors);
    if(base::Time::now() < start + sampling_period){
      //std::cout << "Got new reading on input port: " << sensors.gyro << std::endl;
      x += sensors.gyro[0];
      y += sensors.gyro[1];
      z += sensors.gyro[2];
      ++counter;
      //std::cout << "accumulating, counter: " << counter << std::endl;
    }
    else {
      //base::Orientation orientation;
      //std::cout << "Accumulated x|y|z: " << x <<"|" << y << "|" << z << std::endl;
      double yaw = - atan2(y,x);
      //std::cout<< "Estimated Yaw after north seeking: " << yaw * 180.0 / pi  << std::endl;
      //Eigen::Vector3d geo(0.0,0.0,1.0);
      //Eigen::Vector3d body(x,y,z);
      //orientation.setFromTwoVectors(geo,body);
      _heading.write(yaw);
    }

}
void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}
