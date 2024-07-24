#!/bin/sh
#Start/stop/restart rsync.

# File Name: rsyncd.sh
# Author: Yezhening
# Created Time : 2021-05-24 13:01:55
# Description: rsync service script

#Start rsync:
function rsync_start()
{
	if [ -x /usr/bin/rsync ]; then
		#if there is an old PID file(no rsync running), clean it up:
		if [ -r /var/run/rsyncd.pid ]; then	#Pay attention to the format
			echo "rsync is running."
			if ! ps axc | grep rsync 1 > /dev/null 2 > /dev/null ; then	#Pay attention to the format
				#grep: 1: No such file or directory
				#grep: 2: No such file or directory
				echo "Cleaning up old /var/run/rsync.pid."
				rm -f /var/run/rsyncd.pid
			fi
		fi
		rsync --daemon --config=/etc/rsyncd.conf
	fi
}

#Stop rsync:
function rsync_stop()
{
	#If there is no PID file, ignore this request...
	if [ -r /var/run/rsyncd.pid ]; then
		killall rsync
	# Wait at least one minute for it to exit, as we don't know how big the DB is...
		for second in 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 \
      	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 60 ; 
		do
	  		if [ ! -r /var/run/rsyncd.pid ]; then
	  			break;
	 	 	fi
	 	 	sleep 1
		done
		if [ "$seond" = "60" ]; then
			 echo "WARNING:  Gave up waiting for rsync to exit!"
			sleep 15
		fi
	fi
}

#Restart rsync:
function rsync_restart()
{
	rsync_stop
	rsync_start
}

case "$1" in
'start')
	rsync_start
	;;
'stop')
	rsync_stop
	;;
'restart')
	rsync_restart
	;;
*)
	echo "usage $0 start|stop|restart"	#$0: -bash
esac


