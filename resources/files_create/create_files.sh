#!/bin/bash

STARTTIME=$(date +%s)

if [[ $# -ne 5 ]]; then
	echo "Error : Wrong Number of Arguments." && exit 1;
fi

#if request for an existing directory name, abort
[[ -d $3 ]] && echo "$3 already exists, aborting" && exit  	 

re='^[0-9]+$' 
numFileDir="$4"
numRecFile="$5"

if ! [[ $numFileDir =~ $re ]] ; then echo "Error: Value not a number." >&2; exit 1; fi
if ! [[ $numRecFile =~ $re ]] ; then echo "Error: Value not a number." >&2; exit 1; fi

mkdir $3  #create directory 
countryFile="$2"
diseaseFile="$1"
mindate=2000-1-1
maxdate=$(date '+%Y-%m-%d')
id=1
enter_exit[0]="ENTER"
enter_exit[1]="EXIT"
enters=()
false_entry="0"
entries=()

#returns random date
random_date()
{	
	days=$((($(date +%s --date "$maxdate")-$(date +%s --date "$mindate"))/(3600*24)))
	count=$numFileDir+$i
	if [[ $count -ge $days ]]; then
		random=$((RANDOM%($(($numFileDir-$i)))+1))
	else
		random=$((RANDOM%($days-$numFileDir)+1))
	fi
	day=$(date '+%Y-%m-%d' -d "$mindate + $random day")
	mindate="$day"
	day=$(date -d "$day" '+%d-%m-%Y')
}

#write entries or exits in the date-file
write_file()
{
	for (( k = 0; k < $numRecFile; k++ )); do
		name_len=$((RANDOM%10+3))
		random_index=$((RANDOM%2+0))
		enter_or_exit=${enter_exit[$random_index]}
		#for some false entries (for accurate entries: false_entry=0)
		# false_entry=$((RANDOM%2+0))

		if [  "$enter_or_exit" == "EXIT" ]; then
			#check if enters-array is empty 
			if [ -z "$enters" ]; then
				enter_or_exit="ENTER"
			else
				enter_or_exit="EXIT"
				index=$[$RANDOM % ${#enters[@]}]
				record=${enters[$index]}
				record=${record/ENTER/EXIT}
				unset enters[$index]
				#reindex after delete enters[index] element
				enters=( "${enters[@]}" )
			fi
		fi

		if [  "$enter_or_exit" == "ENTER" ]; then
			if [  "$false_entry" != "0" ]; then
				enter_or_exit="EXIT"
			fi
			name="$(sed "s/[^a-zA-Z0-9]//g" <<< $(cat /dev/urandom | tr -dc 'a-z' | fold -w $name_len | head -n 1))"
			name_len=$((RANDOM%10+3))
			surnname="$(sed "s/[^a-zA-Z0-9]//g" <<< $(cat /dev/urandom | tr -dc 'a-z' | fold -w $name_len | head -n 1))"
			disease=$(shuf -n 1 diseaseFile)
			age=$((RANDOM%120+1))
			#FORMAT: 889 ENTER Mary Smith COVID-2019 23
			record="$id $enter_or_exit $name $surnname $disease $age"
			enters+=("$record")
			id=$((id+1))
		fi
		echo "$k RECORD: $record"

		entries+=("$record")
		# echo "$record" >> "$1"
	done
	
}

#for files with date-name
create_files()
{
	for (( i = 0; i < $numFileDir; i++ )); do
		random_date
		path="../input_dir/$1/$day"
		touch "$path"
		echo "$day"
		write_file "$path"
		if [ "$false_entry" == "0" ]; then
			unset enters
		fi
		printf "%s\n" "${entries[@]}" >> "$path"
		unset entries
	done
}

#for every country from file
while IFS= read -r line || [[ -n "$line" ]] ; do
    name="$line"
    echo "name: $name"
    mkdir -p "../input_dir/$name"
    mindate=2000-3-12
    create_files "$name"
done < "$countryFile"

ENDTIME=$(date +%s)
echo "It takes $(($ENDTIME - $STARTTIME)) seconds to complete this task..."