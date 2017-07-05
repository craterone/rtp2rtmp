BUILDDIR=$HOME/projects/gst
PREFIX=$BUILDDIR/local

mkdir -p $PREFIX

GST_MODULES="gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad "

for MODULE in $GST_MODULES ; do
    cd $BUILDDIR
    git clone git://anongit.freedesktop.org/gstreamer/$MODULE
    cd $BUILDDIR/$MODULE
    git checkout master
    PATH=$PREFIX/bin:$PATH PKG_CONFIG_PATH=$PREFIX/lib/pkgconfig ./autogen.sh --prefix $PREFIX && \
    make && \
    make install
done