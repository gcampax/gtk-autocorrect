srcdir=`dirname $0`

(cd $srcdir;
autoreconf --force --install ${ACLOCAL_FLAGS};
)

${srcdir}/configure $@
