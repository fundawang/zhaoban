#!/bin/bash

BASEDIR=`dirname \`realpath $0\``
NEW_VER=`curl -Ls http://downloads.sourceforge.net/tcpdf/README.TXT |grep Version:|awk -F' ' '{print $2}'`
OLD_VER=`cat $BASEDIR/tcpdf/README.TXT|grep Version:|awk -F' ' '{print $2}'`

if [ "$NEW_VER" != "$OLD_VER" ] ; then
	FILEVER=`echo $NEW_VER|sed -e 's/\./_/g'`
	wget -q http://downloads.sourceforge.net/tcpdf/tcpdf_$FILEVER.zip -O $BASEDIR/tcpdf_$FILEVER.zip
	cd $BASEDIR
	unzip -qqo tcpdf_$FILEVER.zip
	rm -f tcpdf_$FILEVER.zip
fi
