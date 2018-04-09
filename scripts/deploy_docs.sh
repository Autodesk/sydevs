#!/bin/bash
# Based on https://voorhoede.github.io/front-end-tooling-recipes/travis-deploy-to-gh-pages/

set -e # exit with nonzero exit code if anything fails

echo "Starting to update gh-pages\n"

#copy data we're interested in to other place
cp -R doc $HOME/doc

#go to home and setup git
cd $HOME
git config --global user.email "travis@travis-ci.org"
git config --global user.name "Travis"

#using token clone gh-pages branch
git clone --quiet --branch=gh-pages https://${GITHUB_OAUTH_TOKEN}@github.com/Autodesk/sydevs.git gh-pages

#go into directory and copy data we're interested in to that directory
cd gh-pages
cp -Rf $HOME/doc/html/* ./doc/html

#add, commit and push files
git add -f .
git commit -m "Travis build $TRAVIS_BUILD_NUMBER"
git push -fq origin gh-pages

echo "Done updating gh-pages\n"