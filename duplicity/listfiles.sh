#!/bin/bash

# Helper for listfiles
# $1 is the date to query
# $2 is the location of the backup set

source /data/backups/secrets

/data/src/duplicity/duplicity-bin \
    --encrypt-key=${GPG_KEY} \
    --sign-key=${GPG_KEY} \
    --time $1 \
    list-current-files file://$2