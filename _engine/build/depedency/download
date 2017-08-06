#!/bin/bash
cd "$(dirname "$0")"
cd ../..
read -p "This will wipe out existing depedencies and re-download them all. IT WILL TAKE SOME TIME, maybe even a few hours! Are you sure to proceed? y/(n)" -n 1 -r

if [[ $REPLY =~ ^[Yy]$ ]]
then

echo "Deleting existsing depedencies..."

rm -rf ./depedency
mkdir -m777 depedency

echo "Done!"

echo "Downloading depedencies (warning: THIS WILL TAKE SOME TIME!)..."

mkdir -m777 _build
cd ./_build
mkdir -m777 depedency
cd ..

cd ./_build/depedency

curl -o ./depedencies.zip -k "http://open-source.kosek.com/pixie_engine/depedencies.zip"
cd ../..

echo "Done!"

echo "Unpacking depedencies (warning: THIS WILL TAKE !EVEN MORE! TIME!)..."
echo "... like several minutes (or even few hours). Please, wait patiently..."
echo "... you have been warned."

mkdir -m777 depedency
unzip ./_build/depedency/depedencies.zip -d depedency

echo "Done!"

echo "Deleting depedenceis zip file..."

rm -rf ./_build/depedency

echo "Done!"

echo "All done!"

fi