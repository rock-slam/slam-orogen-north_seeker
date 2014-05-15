require 'orocos'
#require 'vizkit'
include Orocos

#if !ARGV[0]
#    STDERR.puts "usage: test.rb <device name>"
#    exit 1
#end

Orocos.initialize

Orocos.run 'imu_kvh_1750::Task' => 'imu', 'north_seeker::Task' => 'seeker' do
#    Orocos.log_all_ports

    Orocos.conf.load_dir('/home/leifole/dev/rock/smallrock/drivers/orogen/imu_kvh_1750/config')
    imu = Orocos.name_service.get 'imu'
    seeker = Orocos.name_service.get 'seeker'

    Orocos.conf.apply(imu, ['default','Bremen','imu_kvh_1750'], :override => true)


    imu.device = "serial:///dev/ttyUSB0:921600"
    imu.timeout = 20 
    imu.use_filter = false

    seeker.sampling_period = 60 
    seeker.configure
    imu.configure
    imu.start
    seeker.start
    imu.raw_sensors.connect_to seeker.imusamples
    puts "done."

    Orocos.watch(imu,seeker)
end

