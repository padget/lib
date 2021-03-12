#!/bin/bash

# Ce module aura pour but de mettre en place 
# une utilisation idempotente du programme git 
# pour le clonage d'un repository git

# --ref reference to checkout after clone
# --repo git, SSH, or HTTP(S) protocol address of the git repository.
# --dest The path of where the repository should be checked out. 
# --accept_hostkey If yes, ensure that "-o StrictHostKeyChecking=no" is present as an ssh option.