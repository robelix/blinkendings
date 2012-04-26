#!/usr/bin/perl -w

use strict;
use Device::SerialPort;
use Time::HiRes qw(usleep nanosleep);
use Imager;
use Data::Dumper;

my $serialDev = '/dev/ttyACM0';
my $serialSpeed = 115200;
my $filename = 'rot.png';

my $outWidth = 8;
my $outHeight = 4;


my $port = Device::SerialPort->new($serialDev) or die $serialDev.' not found - arduino nit eingesteckt?';
$port->databits(8);
$port->baudrate($serialSpeed);
$port->parity("none");
$port->stopbits(1);


my $image = Imager->new;
$image->read(file=>$filename) or die $image->errstr;

print 'width: '.$image->getwidth()."\n";
print 'height: '.$image->getheight()."\n";

my $imgWidth = $image->getwidth();
my $imgHeight = $image->getheight();

my $xOffset = 0;
my $yOffset = 0;


while(1) {
	
	for (my $y = 0; $y < $outHeight; $y++) {
		for (my $x = 0; $x < $outWidth; $x++) {
			
			print 'Offset: '.$xOffset.'x'.$yOffset.' ';
			print 'Pos: '.$x.'x'.$y.' ';
			my $color = $image->getpixel(x=>$x+$xOffset,y=>$y+$yOffset);
			my ($red, $green, $blue, $alpha) = $color->rgba();
			
			print 'R: '.$red.' G: '.$green.' B: '.$blue."\n";
			
			nanosleep(50000);
			$port->write( chr($x+$y*$outWidth). chr($red/10). chr($green/10). chr($blue/10) );

			nanosleep(50000);
			$port->write( chr( ($x+$y*$outWidth) +16). chr($red/10). chr($green/10). chr($blue/10) );

			nanosleep(50000);
			$port->write( chr( ($x+$y*$outWidth) +32). chr($red/10). chr($green/10). chr($blue/10) );

			nanosleep(50000);
			$port->write( chr( ($x+$y*$outWidth) +48). chr($red/10). chr($green/10). chr($blue/10) );

			nanosleep(50000);
			$port->write( chr( ($x+$y*$outWidth) +64). chr($red/10). chr($green/10). chr($blue/10) );

			nanosleep(50000);
			$port->write( chr( ($x+$y*$outWidth) +80). chr($red/10). chr($green/10). chr($blue/10) );
}
	}

	print 'RESET'."\n";
	nanosleep(50000);
	$port->write( chr(255).chr(255).chr(255).chr(255) );

	
	$xOffset++;
	
	if ($xOffset + $outWidth > $imgWidth) {
		$xOffset = 0;
		$yOffset = $yOffset+$outHeight;
		
		if ($yOffset + $outHeight > $imgHeight) {
			$xOffset = 0;
			$yOffset = 0
		}
	}
	
	nanosleep(20000000);
}


my $masks = {
	
}

#sub getLedNr {
#	my $x = shift;
#	my $y = shift;
#	
#	
#}
