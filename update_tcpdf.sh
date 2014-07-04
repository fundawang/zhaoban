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

if [ ! -f tcpdf/fonts/droidsansfallback.z ] ; then
	wget -q https://github.com/android/platform_frameworks_base/raw/master/data/fonts/DroidSansFallback.ttf -O tcpdf/fonts/droidsansfallback.ttf
	cat > tcpdf/examples/examples_066.php << EOF
<?php
require_once("tcpdf_include.php");
\$pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false, true);
\$pdf->addTTFfont('../fonts/droidsansfallback.ttf', 'TrueTypeUnicode', '', 32);
?>
EOF
	cd tcpdf/examples
	php examples_066.php
	cd ../..
fi
