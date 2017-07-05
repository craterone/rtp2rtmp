BUILDDIR=$HOME/gst
VERSION=1.12.1
mkdir -p $BUILDDIR

GST_MODULES="gstreamer gst-plugins-base gst-plugins-bad gst-plugins-good gst-plugins-ugly gst-libav gstreamer-vaapi"

for MODULE in $GST_MODULES ; do
    cd $BUILDDIR
    #git clone git://anongit.freedesktop.org/gstreamer/$MODULE
    wget https://gstreamer.freedesktop.org/src/$MODULE/$MODULE-$VERSION.tar.xz
    tar xf $MODULE-$VERSION.tar.xz
    cd $BUILDDIR/$MODULE-$VERSION
    ./autogen.sh --noconfigure && ./configure --prefix=/usr && \
    make && \
    make install
done

rm -rf $BUILDDIR/*
