# Create Simulator
set ns [new Simulator]

# Create Trace Files
set nr [open tr1.tr w]
$ns trace-all $nr

set nf [open tr1.nam w]
$ns namtrace-all $nf


# Create Nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]


# Create Links
$ns duplex-link $n0 $n2 1Mb 10ms DropTail
$ns duplex-link $n6 $n2 1Mb 10ms DropTail
$ns duplex-link $n1 $n2 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 3Mb 10ms DropTail
$ns duplex-link $n3 $n7 2Mb 10ms DropTail
$ns duplex-link $n3 $n4 2Mb 10ms DropTail
$ns duplex-link $n3 $n5 2Mb 10ms DropTail


# Set Link Orientation (for NAM)
$ns duplex-link-op $n0 $n2 orient right-down
$ns duplex-link-op $n6 $n2 orient right
$ns duplex-link-op $n1 $n2 orient right-up
$ns duplex-link-op $n2 $n3 orient right
$ns duplex-link-op $n3 $n5 orient right-up
$ns duplex-link-op $n3 $n7 orient right
$ns duplex-link-op $n3 $n4 orient right-down


# TCP Connection 1
set tcp0 [new Agent/TCP]
set sink5 [new Agent/TCPSink]

$ns attach-agent $n0 $tcp0
$ns attach-agent $n5 $sink5
$ns connect $tcp0 $sink5

$tcp0 set fid_ 2
$ns color 2 Black

set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0


# TCP Connection 2
set tcp1 [new Agent/TCP]
set sink7 [new Agent/TCPSink]

$ns attach-agent $n6 $tcp1
$ns attach-agent $n7 $sink7
$ns connect $tcp1 $sink7

set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1


# UDP Connection
set udp0 [new Agent/UDP]
set null0 [new Agent/Null]

$ns attach-agent $n1 $udp0
$ns attach-agent $n4 $null0
$ns connect $udp0 $null0


# CBR Traffic
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 500
$cbr0 set interval_ 0.005
$cbr0 attach-agent $udp0


# Traffic Start and Stop Times
$ns at 0.1 "$cbr0 start"
$ns at 4.5 "$cbr0 stop"

$ns at 1.0 "$ftp0 start"
$ns at 4.0 "$ftp0 stop"

$ns at 1.5 "$ftp1 start"
$ns at 4.2 "$ftp1 stop"


# Finish Procedure
proc finish {} {
    global ns nf nr
    $ns flush-trace
    close $nf
    close $nr
    exec nam tr1.nam &
    exit 0
}


# Schedule Finish
$ns at 5.0 "finish"


# Run Simulation
$ns run

