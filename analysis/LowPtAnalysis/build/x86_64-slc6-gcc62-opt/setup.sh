# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
#
# Script providing a functional build and runtime environment for project
# UserAnalysis when sourced from either BASH or ZSH.
#

# Check if we are in BASH. In that case determining the directory holding
# the script is relatively easy:
if [ "x${BASH_SOURCE[0]}" = "x" ]; then
    # This trick should do the right thing under ZSH:
    thisdir=$(dirname `print -P %x`)
    if [ $? != 0 ]; then
        echo "ERROR: This script must be sourced from BASH or ZSH"
        return 1
    fi
    # We are in ZSH, so feel free to use a ZSH specific formalism:
    if [[ $options[shwordsplit] != on ]]; then
        # Instruct ZSH to behave like BASH does while iterating over
        # space separated words:
        setopt shwordsplit
        UserAnalysis_UNSETSHWORDSPLIT=1
    fi
else
    # The BASH solution is a bit more straight forward:
    thisdir=$(dirname ${BASH_SOURCE[0]})
fi

# Function removing the duplicates from a given environment variable
remove_duplicates() {
    # Make it clear that the function's argument is the name of the
    # environment variable:
    envname=$1
    # Make copy of the environment variable:
    eval "temp=\$$envname"
    # Reset the value of the environment variable:
    eval "${envname}="
    # Loop over the elements of the copy:
    while [ -n "${temp}" ]; do
        # Take the first element of the remaining list:
        x=${temp%%:*}
        # Check that it's not an empty string:
        if [ "$x" != "" ]; then
            # If it's an existing directory, get its absolute path:
            if [ -d "$x" ] && [ -r "$x" ] && [ -x "$x" ]; then
                x=$(\cd "$x";\pwd)
            fi
            # Get the current value of the environment variable:
            eval "envval=\$$envname"
            # Decide whether to add it to the environment or not:
            case ${envval}: in
                *:"$x":*) ;;
                *) eval "${envname}=\"${envval}:$x\"";;
            esac
        fi
        # Remove the element from the list:
        temp=${temp#*:}
    done
    # Remove the leading ":" from the path:
    eval "envval=\$$envname"
    envval=`echo $envval | cut -c2-`
    eval "${envname}=\"${envval}\""
    # Export it:
    export ${envname}
    # And finally clean up:
    unset temp x envname envval
}

# Decide about the environment status variable to be used:
ENV_VAR=UserAnalysis_SET_UP
if [ "$*" = "extonly" ]; then
    ENV_VAR=UserAnalysis_EXTONLY_SET_UP
elif [ "$*" = "relonly" ]; then
    ENV_VAR=UserAnalysis_RELONLY_SET_UP
fi
# Check what its current value is:
eval "ENV_VAR_VALUE=\$$ENV_VAR"

# Prevent the script from being called multiple times.
if [ "x${ENV_VAR_VALUE}" = "x" ]; then
    # Set the variable now:
    eval "export ${ENV_VAR}=1"

    # Set up the project's main directory:
    export UserAnalysis_DIR=$(\cd ${thisdir};\pwd)
    unset thisdir

    # Extract directory names relevant for the rest of the operations:
    export UserAnalysis_PLATFORM=$(basename ${UserAnalysis_DIR})
    export UserAnalysis_VERSION=$(basename $(dirname $(dirname ${UserAnalysis_DIR})))

    # Base projects used for the build of UserAnalysis:
    UserAnalysis_BASEPROJECTS=" AnalysisBase"

    # If the project has base projects, set up those first. First, we just set
    # up the external environment of the base projects:
    if [ "$*" != "relonly" ]; then
        for project in ${UserAnalysis_BASEPROJECTS}; do
            # Strip off the whitespaces from the project name:
            p="$(echo ${project} | xargs)"
            # Name of the setup file in the base release:
            fname=${UserAnalysis_DIR}/../../../../${p}/${UserAnalysis_VERSION}/InstallArea/${UserAnalysis_PLATFORM}/setup.sh
            if [ -f ${fname} ]; then
                . ${fname} extonly
            fi
            unset fname p
        done
    fi

    # Source the environment setup script, when we're not in release-only mode:
    if [ "$*" != "relonly" ]; then
        # The new name for the script is env_setup.sh. But let's keep looking
        # for lcg_setup.sh a little longer, until all the projects have migrated
        # to the new naming convention.
        if [ -f ${UserAnalysis_DIR}/env_setup.sh ]; then
            . ${UserAnalysis_DIR}/env_setup.sh
        elif [ -f ${UserAnalysis_DIR}/lcg_setup.sh ]; then
            . ${UserAnalysis_DIR}/lcg_setup.sh
        fi
    fi

    # If we are in "external only" mode, then don't do any of the rest.
    if [ "$*" != "extonly" ]; then

        # Now source the setup.sh file(s) of the base release(s), in
        # release-only mode:
        for project in ${UserAnalysis_BASEPROJECTS}; do
            # Strip off the whitespaces from the project name:
            p="$(echo ${project} | xargs)"
            # Name of the setup file in the base release:
            fname=${UserAnalysis_DIR}/../../../../${p}/${UserAnalysis_VERSION}/InstallArea/${UserAnalysis_PLATFORM}/setup.sh
            if [ -f ${fname} ]; then
                . ${fname} relonly
            fi
            unset fname p
        done

        # Set up the release's main directories. Note that the names must
        # include ":" as the last character, for the remove_duplicates function
        # to work correctly.
        if [ -z "${CMAKE_PREFIX_PATH}" ]; then
            export CMAKE_PREFIX_PATH=${UserAnalysis_DIR}:
        else
            export CMAKE_PREFIX_PATH=${UserAnalysis_DIR}:${CMAKE_PREFIX_PATH}:
        fi

        # Set up the binary path(s):
        if [ -z "${PATH}" ]; then
            export PATH=${UserAnalysis_DIR}/bin:
        else
            export PATH=${UserAnalysis_DIR}/bin:${PATH}:
        fi
        export PATH=${UserAnalysis_DIR}/share:${PATH}:

        # Set up the library path(s):
        if [ -z "${LD_LIBRARY_PATH}" ]; then
            export LD_LIBRARY_PATH=${UserAnalysis_DIR}/lib:
        else
            export LD_LIBRARY_PATH=${UserAnalysis_DIR}/lib:${LD_LIBRARY_PATH}:
        fi
        if [ -z "${DYLD_LIBRARY_PATH}" ]; then
            export DYLD_LIBRARY_PATH=${UserAnalysis_DIR}/lib:
        else
            export DYLD_LIBRARY_PATH=${UserAnalysis_DIR}/lib:${DYLD_LIBRARY_PATH}:
        fi

        # Set up the python path(s):
        if [ -z "${PYTHONPATH}" ]; then
            export PYTHONPATH=${UserAnalysis_DIR}/lib:
        else
            export PYTHONPATH=${UserAnalysis_DIR}/lib:${PYTHONPATH}:
        fi
        export PYTHONPATH=${UserAnalysis_DIR}/python:${PYTHONPATH}:

        # Set up the jobOptions path:
        if [ -z "${JOBOPTSEARCHPATH}" ]; then
            export JOBOPTSEARCHPATH=${UserAnalysis_DIR}/jobOptions:
        else
            export JOBOPTSEARCHPATH=${UserAnalysis_DIR}/jobOptions:${JOBOPTSEARCHPATH}:
        fi

        # Set up the data path(s):
        if [ -z "${DATAPATH}" ]; then
            export DATAPATH=${UserAnalysis_DIR}/share:
        else
            export DATAPATH=${UserAnalysis_DIR}/share:${DATAPATH}:
        fi
        export DATAPATH=${UserAnalysis_DIR}/data:${DATAPATH}:
        if [ -z "${CALIBPATH}" ]; then
            export CALIBPATH=${UserAnalysis_DIR}/share:
        else
            export CALIBPATH=${UserAnalysis_DIR}/share:${CALIBPATH}:
        fi
        export CALIBPATH=${UserAnalysis_DIR}/data:${CALIBPATH}:

        # Set up the include path:
        if [ -z "${ROOT_INCLUDE_PATH}" ]; then
            export ROOT_INCLUDE_PATH=${UserAnalysis_DIR}/include:
        else
            export ROOT_INCLUDE_PATH=${UserAnalysis_DIR}/include:${ROOT_INCLUDE_PATH}:
        fi

        # Set up the XML path:
        if [ -z "${XMLPATH}" ]; then
            export XMLPATH=${UserAnalysis_DIR}/XML:
        else
            export XMLPATH=${UserAnalysis_DIR}/XML:${XMLPATH}:
        fi

        # We can already not be in external-only mode. But the rest should only
        # be done if we are also not in release-only mode.
        if [ "$*" != "relonly" ]; then

            # Remove the duplicates from all the environment variables that were
            # touched by the script:
            remove_duplicates CMAKE_PREFIX_PATH
            remove_duplicates PATH
            remove_duplicates LD_LIBRARY_PATH
            remove_duplicates DYLD_LIBRARY_PATH
            remove_duplicates PYTHONPATH
            remove_duplicates JOBOPTSEARCHPATH
            remove_duplicates DATAPATH
            remove_duplicates CALIBPATH
            remove_duplicates ROOT_INCLUDE_PATH
            remove_duplicates XMLPATH

            # Add the current directory as well to JOBOPTSEARCHPATH:
            export JOBOPTSEARCHPATH=.:${JOBOPTSEARCHPATH}

        fi
    fi
fi

# Clean up:
unset ENV_VAR
if [ ! -z "${UserAnalysis_UNSETSHWORDSPLIT}" ]; then
    unsetopt shwordsplit
    unset UserAnalysis_UNSETSHWORDSPLIT
fi
