# Copyright (c) 2018 Institute of Sound and Vibration Research, University of Southampton.
# All rights reserved.

# Doxygen documentation building
# Even though Doxygen-generated documentation will not be deployed in the long term,
# the Sphinx-generated C++ API documentation uses Doxygen in the background to extract information
# from C++ code and stores it in XML format, which is picked up by the Breathe for Sphinx package

find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
  #TODO add installation line guides for installing Doxygen via website www. ....
  message(FATAL_ERROR "Doxygen is needed to build the documentation. You can install it here at ...")
endif()

# Latex is used to create PDFs from the sphinx documentation
find_package( LATEX COMPONENTS PDFLATEX )

set( doxyfile_in          ${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile.in )
set( doxyfile             ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile )
set( doxy_html_index_file ${PROJECT_BINARY_DIR}/doc/doxygen/html/index.html )
set( doxy_output_root     ${PROJECT_BINARY_DIR}/doc/doxygen/ )
set( doxy_input           ${PROJECT_SOURCE_DIR}/src )
set( doxy_extra_files     ${PROJECT_SOURCE_DIR}/doc/doxygen/ )
# Used for checking dependencies when using Sphinx custom commands
set( doxy_xml_index_file ${PROJECT_BINARY_DIR}/doc/doxygen/xml/index.xml )
# Used as a varible config.py.in with Sphinx to configure the breathe extension.
set( doxy_xml_index       ${PROJECT_BINARY_DIR}/doc/doxygen/xml/ )

configure_file( ${doxyfile_in} ${doxyfile} @ONLY )

add_custom_command( OUTPUT ${doxy_html_index_file}
		  COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		  # The following should be ${doxyfile} only but it
		  # will break the dependency.
		  # The optimal solution would be creating a
		  # custom_command for ${doxyfile} generation
		  # but I still have to figure out how...
		  MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
		  DEPENDS ${doxy_extra_files} # TODO <AF> Decide whether the subprojects should be added as dependency.
		  COMMENT "Generating Doxygen HTML and XML documentation")
		  
add_custom_target( doxygen-doc DEPENDS ${doxy_html_index_file}  )

# "Meta-target" to trigger build of all documentation outputs
add_custom_target( doc ALL DEPENDS doxygen-doc )

find_package(Sphinx)
if(NOT Sphinx_FOUND)
  message(FATAL_ERROR "Sphinx is required to build documentation. Please set the SPHINX_EXECUTABLE variable.")
endif(NOT Sphinx_FOUND)

# TODO Do we need this ? Look in virtualenv and node_modules directories of the build directory (as
# per the hints in the documentation about installing sphinx, breathe,
# sphinx_js and jsdoc).
set(BUILD_VIRTUALENV_PATH ${PROJECT_BINARY_DIR}/venv/bin/)
list(INSERT CMAKE_PROGRAM_PATH 0 ${BUILD_VIRTUALENV_PATH})


set (userdoc_source_root    ${PROJECT_SOURCE_DIR}/doc/userdoc/)
set (apidoc_source_root    ${PROJECT_SOURCE_DIR}/doc/apidoc/)
set( userdoc_output_root   ${PROJECT_BINARY_DIR}/doc/userdoc )
set( apidoc_output_root   ${PROJECT_BINARY_DIR}/doc/apidoc )

if(NOT DEFINED SPHINX_THEME)
    set(SPHINX_THEME default)
endif()

if(NOT DEFINED SPHINX_THEME_DIR)
    set(SPHINX_THEME_DIR)
endif()

# Function to define a target building HTML documentation from a source directory containing Sphinx rst code.
function( HtmlDocBuild targetName sourceDir outputDir depends )
  configure_file( ${sourceDir}/conf.py.in ${outputDir}/html/conf.py @ONLY )
  # Define an empty file that gets updated when the target is build, such that it can be used as
  # a dependency for other targets. 
  set( STAMP ${outputDir}/htmlbuild.stamp )
  add_custom_command(
    OUTPUT ${STAMP}
    DEPENDS doxygen-doc ${outputDir}/html/conf.py ${sourceDir}
    COMMAND ${SPHINX_EXECUTABLE}
               -q
               -b html
               -c ${outputDir}/html
               -d ${outputDir}/cache
                ${sourceDir}
                ${outputDir}/html
    COMMAND cmake -E touch ${STAMP}
    BYPRODUCTS ${outputDir}/cache
    COMMENT "HTML generation for target ${targetName}-html."
  )
  add_custom_target( ${targetName}-html
    DEPENDS ${STAMP}
  )
endfunction(HtmlDocBuild)

# Function to define a target building PDF documentation from a source directory containing Sphinx rst code.
function( PdfDocBuild targetName sourceDir outputDir depends )
  configure_file( ${sourceDir}/conf.py.in ${outputDir}/latex/conf.py @ONLY )
  # Name (with version) of the output file. This is the main dependency of the target.
set( OUTPUTFILE ${outputDir}/${targetName}-${VISR_PRODUCTION_SUITE_MAJOR_VERSION}.${VISR_PRODUCTION_SUITE_MINOR_VERSION}.${VISR_PRODUCTION_SUITE_PATCH_VERSION}.pdf )
  add_custom_command(
    OUTPUT ${OUTPUTFILE}
    DEPENDS doxygen-doc ${outputDir}/latex/conf.py ${sourceDir}
    COMMAND ${SPHINX_EXECUTABLE}
               -q
               -b latex
               -c ${outputDir}/latex
               -d ${outputDir}/cache
                ${sourceDir}
                ${outputDir}/latex
    COMMAND ${PDFLATEX_COMPILER} -halt-on-error visr-production-suite
    COMMAND ${PDFLATEX_COMPILER} -halt-on-error visr-production-suite
    COMMAND ${PDFLATEX_COMPILER} -halt-on-error visr-production-suite
    COMMAND ${CMAKE_COMMAND} -E rename ${outputDir}/latex/visr-production-suite.pdf ${outputDir}/${targetName}-${VISR_PRODUCTION_SUITE_MAJOR_VERSION}.${VISR_PRODUCTION_SUITE_MINOR_VERSION}.${VISR_PRODUCTION_SUITE_PATCH_VERSION}.pdf
    WORKING_DIRECTORY ${outputDir}/latex
    BYPRODUCTS ${outputDir}/cache ${outputDir}/latex
    COMMENT "PDF compilation for target ${targetName}-pdf."
  )
  add_custom_target( ${targetName}-pdf
    DEPENDS ${OUTPUTFILE}
  )
endfunction(PdfDocBuild)

PdfDocBuild( userdoc ${userdoc_source_root} ${userdoc_output_root} doxygen-doc )
PdfDocBuild( apidoc ${apidoc_source_root} ${apidoc_output_root} doxygen-doc )

HtmlDocBuild( userdoc ${userdoc_source_root} ${userdoc_output_root} doxygen-doc )
HtmlDocBuild( apidoc ${apidoc_source_root} ${apidoc_output_root} doxygen-doc )

# Add the Sphinx-generated targets to the "doc" meta-target
add_dependencies( doc apidoc-html apidoc-pdf userdoc-html userdoc-pdf ) 

# Include the Doxygen documentation in the package installer
# Note: Doxygen as output format is deprecated, so it won't be externally visible in the future.
#install( DIRECTORY ${PROJECT_BINARY_DIR}/doc/doxygen/html DESTINATION ${DOCUMENTATION_INSTALL_DIRECTORY} )

# Place the generated PDF documentation in the installers.
# NOTE: Unfortuanately we need to operate on files, as installing for targets apparently works only for executables and libraries (which have a predefined main file)
# TODO: Remove construction of file name with full version string.
install( FILES  ${userdoc_output_root}/userdoc-${VISR_PRODUCTION_SUITE_MAJOR_VERSION}.${VISR_PRODUCTION_SUITE_MINOR_VERSION}.${VISR_PRODUCTION_SUITE_PATCH_VERSION}.pdf DESTINATION ${DOCUMENTATION_INSTALL_DIRECTORY} )
#install( FILES  ${apidoc_output_root}/apidoc-${VISR_PRODUCTION_SUITE_MAJOR_VERSION}.${VISR_PRODUCTION_SUITE_MINOR_VERSION}.${VISR_PRODUCTION_SUITE_PATCH_VERSION}.pdf DESTINATION ${DOCUMENTATION_INSTALL_DIRECTORY} )

