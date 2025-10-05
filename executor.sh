#!/usr/bin/bash

CMD="WINEPREFIX=/home/mico/.wineprefixes/ruslanas-photo-account wine build/ruslanas-photo-account.exe && bash"

# Open a new terminal and run the command
konsole -e bash -c "$CMD"
