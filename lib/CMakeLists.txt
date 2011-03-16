## This file is part of mccparser.
##
## mccparser is a parser utility tools for MC-Sym
## analysis and manipulation of RNA, DNA and protein 3D structures
## Copyright (C) 2008,2009,2010,2011 Université de Montréal
##

cmake_minimum_required (VERSION 2.8)

# gère la dépendance bison/flex
bison_target(MCCPARSER mccp.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-y -p mcc -d -t --report=all")
flex_target(MCCFLEXER mccp.yy ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp COMPILE_FLAGS "-Pmcc")
add_flex_bison_dependency(MCCFLEXER MCCPARSER)

if (${BISON_MCCPARSER_DEFINED} AND ${FLEX_MCCFLEXER_DEFINED})
    # liste de tous les fichiers source
    file(GLOB MCCPARSER_SOURCES_CC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cc)
    
    # ajoute la librarie
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    add_library (mccparser ${MCCPARSER_SOURCES_CC} ${BISON_MCCPARSER_OUTPUTS} ${FLEX_MCCFLEXER_OUTPUTS})
    set_target_properties(mccparser PROPERTIES VERSION ${MCCPARSER_VERSION_STRING})

    # ajoute le target d'installation
    install (TARGETS mccparser DESTINATION lib${LIB_SUFFIX})
    install(DIRECTORY ./ DESTINATION include/${MCCPARSER_COMPLETE_PACKAGE_NAME} FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)

    # wrap dans un install(CODE) pour que la commande soit exécuter à l'installation
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/include/${MCCPARSER_COMPLETE_PACKAGE_NAME} ${CMAKE_INSTALL_PREFIX}/include/${PACKAGE_NAME})")

else()
  message(FATAL "Bison/Flex operations failed")
endif()


