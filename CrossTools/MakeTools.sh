#! /bin/bash
SrcDir=/home/liuhao/ToolsSource
ToolsName=crosstool-ng-1.19.0
TupleName=mips-unknown-linux-uclibc
SampleName=mips-unknown-linux-uclibc
#TupleName=mips-unknown-linux-gnu
#SampleName=mipsel-unknown-linux-gnu

###### Important, Step ########
#http://permalink.gmane.org/gmane.comp.gcc.cross-compiling/16081
#sudo yum install expat-devel
#to avoid error: python is missing or unusable
#sudo yum install python-devel
#sudo yum install glibc-static

if [ $# != 1 ]; then
    goto ErrParameter
fi
option=$1
case $option in
clean) echo "Remove building directory"
    sudo rm -rf /opt/${TupleName}
    rm -rf Configure
    ;;
cross-ng) echo "Install $ToolsName"
    rm -rf ${ToolsName} && rm -rf /opt/NgTools
    tar -xvf ${SrcDir}/${ToolsName}.tar.bz2
    pushd ${ToolsName}
    ./configure --prefix=/opt/NgTools
    make && make install
    popd
    rm -rf ${ToolsName}
    ;;
configure) echo "Configure TupleName: ${TupleName}"
    mkdir Configure
    pushd Configure
    ct-ng ${SampleName}
    popd
    sed -i -e 's#^CT_LOCAL_TARBALLS_DIR.*$#CT_LOCAL_TARBALLS_DIR="${HOME}/ToolsSource"#' \
      -e 's#^CT_WORK_DIR=.*$#CT_WORK_DIR="${CT_TOP_DIR}"#' \
      -e 's#^CT_PREFIX_DIR=.*$#CT_PREFIX_DIR="/opt/'''${TupleName}'''"#' \
      -e 's#^CT_ARCH_ARCH=.*$#CT_ARCH_ARCH="4kc"#' \
      -e 's#^CT_TARGET_VENDOR=.*$#CT_TARGET_VENDOR="unknown"#' \
      Configure/.config
    ;;
menuconfigure)
    #if SampleName=mipsel-unknown-linux-gnu, D¨¨¨°a¨ª¡§1ymenuconfigDT?? BigEndian, GCC version ?a4.4.4 ¡¤??¨°¡À¨¤¨°?2?1y.
    pushd Configure
    ct-ng menuconfig
    popd
    ;;
build)
    pushd Configure
    ct-ng build.4
    popd
    ;;
*)
    goto ErrParameter
    ;;
esac
exit 0
ErrParameter:
    echo "Wrong paramter! clean, cross-ng, configure, menuconfigure, build!"
    exit 1
