# Install script for directory: /home/yong/cutefish-screenshot

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/cutefish-screenshot/translations/ar_AA.qm;/usr/share/cutefish-screenshot/translations/az_AZ.qm;/usr/share/cutefish-screenshot/translations/be_BY.qm;/usr/share/cutefish-screenshot/translations/be_Latn.qm;/usr/share/cutefish-screenshot/translations/bg_BG.qm;/usr/share/cutefish-screenshot/translations/bn_BD.qm;/usr/share/cutefish-screenshot/translations/bs_BA.qm;/usr/share/cutefish-screenshot/translations/cs_CZ.qm;/usr/share/cutefish-screenshot/translations/da_DK.qm;/usr/share/cutefish-screenshot/translations/de_DE.qm;/usr/share/cutefish-screenshot/translations/en_US.qm;/usr/share/cutefish-screenshot/translations/eo_XX.qm;/usr/share/cutefish-screenshot/translations/es_ES.qm;/usr/share/cutefish-screenshot/translations/es_MX.qm;/usr/share/cutefish-screenshot/translations/fa_IR.qm;/usr/share/cutefish-screenshot/translations/fi_FI.qm;/usr/share/cutefish-screenshot/translations/fr_FR.qm;/usr/share/cutefish-screenshot/translations/got.qm;/usr/share/cutefish-screenshot/translations/he_IL.qm;/usr/share/cutefish-screenshot/translations/hi_IN.qm;/usr/share/cutefish-screenshot/translations/hr_HR.qm;/usr/share/cutefish-screenshot/translations/hu_HU.qm;/usr/share/cutefish-screenshot/translations/id_ID.qm;/usr/share/cutefish-screenshot/translations/ie.qm;/usr/share/cutefish-screenshot/translations/it_IT.qm;/usr/share/cutefish-screenshot/translations/ja_JP.qm;/usr/share/cutefish-screenshot/translations/lt_LT.qm;/usr/share/cutefish-screenshot/translations/lv_LV.qm;/usr/share/cutefish-screenshot/translations/mg.qm;/usr/share/cutefish-screenshot/translations/ml_IN.qm;/usr/share/cutefish-screenshot/translations/nb_NO.qm;/usr/share/cutefish-screenshot/translations/ne_NP.qm;/usr/share/cutefish-screenshot/translations/nl_NL.qm;/usr/share/cutefish-screenshot/translations/pl_PL.qm;/usr/share/cutefish-screenshot/translations/pt_BR.qm;/usr/share/cutefish-screenshot/translations/pt_PT.qm;/usr/share/cutefish-screenshot/translations/ro_RO.qm;/usr/share/cutefish-screenshot/translations/ru_RU.qm;/usr/share/cutefish-screenshot/translations/si_LK.qm;/usr/share/cutefish-screenshot/translations/sk_SK.qm;/usr/share/cutefish-screenshot/translations/so.qm;/usr/share/cutefish-screenshot/translations/sr_RS.qm;/usr/share/cutefish-screenshot/translations/sv_SE.qm;/usr/share/cutefish-screenshot/translations/sw.qm;/usr/share/cutefish-screenshot/translations/ta_IN.qm;/usr/share/cutefish-screenshot/translations/tg_TJ.qm;/usr/share/cutefish-screenshot/translations/tr_TR.qm;/usr/share/cutefish-screenshot/translations/tt_RU.qm;/usr/share/cutefish-screenshot/translations/tzm.qm;/usr/share/cutefish-screenshot/translations/uk_UA.qm;/usr/share/cutefish-screenshot/translations/uz_UZ.qm;/usr/share/cutefish-screenshot/translations/vi_VN.qm;/usr/share/cutefish-screenshot/translations/zh_CN.qm;/usr/share/cutefish-screenshot/translations/zh_Hant.qm;/usr/share/cutefish-screenshot/translations/zh_Hant_HK.qm")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/cutefish-screenshot/translations" TYPE FILE FILES
    "/home/yong/cutefish-screenshot/build/ar_AA.qm"
    "/home/yong/cutefish-screenshot/build/az_AZ.qm"
    "/home/yong/cutefish-screenshot/build/be_BY.qm"
    "/home/yong/cutefish-screenshot/build/be_Latn.qm"
    "/home/yong/cutefish-screenshot/build/bg_BG.qm"
    "/home/yong/cutefish-screenshot/build/bn_BD.qm"
    "/home/yong/cutefish-screenshot/build/bs_BA.qm"
    "/home/yong/cutefish-screenshot/build/cs_CZ.qm"
    "/home/yong/cutefish-screenshot/build/da_DK.qm"
    "/home/yong/cutefish-screenshot/build/de_DE.qm"
    "/home/yong/cutefish-screenshot/build/en_US.qm"
    "/home/yong/cutefish-screenshot/build/eo_XX.qm"
    "/home/yong/cutefish-screenshot/build/es_ES.qm"
    "/home/yong/cutefish-screenshot/build/es_MX.qm"
    "/home/yong/cutefish-screenshot/build/fa_IR.qm"
    "/home/yong/cutefish-screenshot/build/fi_FI.qm"
    "/home/yong/cutefish-screenshot/build/fr_FR.qm"
    "/home/yong/cutefish-screenshot/build/got.qm"
    "/home/yong/cutefish-screenshot/build/he_IL.qm"
    "/home/yong/cutefish-screenshot/build/hi_IN.qm"
    "/home/yong/cutefish-screenshot/build/hr_HR.qm"
    "/home/yong/cutefish-screenshot/build/hu_HU.qm"
    "/home/yong/cutefish-screenshot/build/id_ID.qm"
    "/home/yong/cutefish-screenshot/build/ie.qm"
    "/home/yong/cutefish-screenshot/build/it_IT.qm"
    "/home/yong/cutefish-screenshot/build/ja_JP.qm"
    "/home/yong/cutefish-screenshot/build/lt_LT.qm"
    "/home/yong/cutefish-screenshot/build/lv_LV.qm"
    "/home/yong/cutefish-screenshot/build/mg.qm"
    "/home/yong/cutefish-screenshot/build/ml_IN.qm"
    "/home/yong/cutefish-screenshot/build/nb_NO.qm"
    "/home/yong/cutefish-screenshot/build/ne_NP.qm"
    "/home/yong/cutefish-screenshot/build/nl_NL.qm"
    "/home/yong/cutefish-screenshot/build/pl_PL.qm"
    "/home/yong/cutefish-screenshot/build/pt_BR.qm"
    "/home/yong/cutefish-screenshot/build/pt_PT.qm"
    "/home/yong/cutefish-screenshot/build/ro_RO.qm"
    "/home/yong/cutefish-screenshot/build/ru_RU.qm"
    "/home/yong/cutefish-screenshot/build/si_LK.qm"
    "/home/yong/cutefish-screenshot/build/sk_SK.qm"
    "/home/yong/cutefish-screenshot/build/so.qm"
    "/home/yong/cutefish-screenshot/build/sr_RS.qm"
    "/home/yong/cutefish-screenshot/build/sv_SE.qm"
    "/home/yong/cutefish-screenshot/build/sw.qm"
    "/home/yong/cutefish-screenshot/build/ta_IN.qm"
    "/home/yong/cutefish-screenshot/build/tg_TJ.qm"
    "/home/yong/cutefish-screenshot/build/tr_TR.qm"
    "/home/yong/cutefish-screenshot/build/tt_RU.qm"
    "/home/yong/cutefish-screenshot/build/tzm.qm"
    "/home/yong/cutefish-screenshot/build/uk_UA.qm"
    "/home/yong/cutefish-screenshot/build/uz_UZ.qm"
    "/home/yong/cutefish-screenshot/build/vi_VN.qm"
    "/home/yong/cutefish-screenshot/build/zh_CN.qm"
    "/home/yong/cutefish-screenshot/build/zh_Hant.qm"
    "/home/yong/cutefish-screenshot/build/zh_Hant_HK.qm"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cutefish-screenshot" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cutefish-screenshot")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cutefish-screenshot"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/yong/cutefish-screenshot/build/cutefish-screenshot")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cutefish-screenshot" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cutefish-screenshot")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/cutefish-screenshot")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/share/applications/cutefish-screenshot.desktop")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/share/applications" TYPE FILE FILES "/home/yong/cutefish-screenshot/cutefish-screenshot.desktop")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/yong/cutefish-screenshot/build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/yong/cutefish-screenshot/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
