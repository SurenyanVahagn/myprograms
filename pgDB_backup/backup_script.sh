#!bin/bash

LOG_FILE = "$BACKUP_PATH/$BACKUP_PREFIX.$(data +%Y.%m.%d_%H-%M-%D)"


log_message() {
    local message=$1
    echo "[$(data +%Y.%m.%d_%H-%M-%D)] $message"| tee -a "$LOG_FILE"
}

remove() {
    local backups_count=$(ls -1 $BACKUP_PATH/$BACKUP_PREFIX.*.tar.lz | wc -l)
    if [backups_count -ge $BACKUP_COUNT]; then
        local oldest=$(ls -1 $BACKUP_PATH/$BACKUP_PREFIX.*.tar.lz | tail -1)
        echo "$oldest" | xargs rm -f
    fi
}

if [$# -ne 2]; then
    echo "You need give script addres!"
    log_message "Config error!!!"
    exit 1
fi

CONFIG_=$1

if [! -f "$CONFIG_"]; then
    echo "There is no config file"
    exit 1
fi

# Беру переменные из конйфиг файла
source "$CONFIG_"

log_message "Starting"

BACKUP_FILE = "$BACKUP_PATH/$BACKUP_PREFIX.$(data +%Y.%m.%d_%H-%M-%D)"


available_space=$(df -P "$BACKUP_PATH" --output=avail | tail -1)
if [ "$available_space" -lt "$REQUIRED_SPACE" ]; then
    log_message "Not enough space!!!"
    log_message "Required $REQUIRED_SPACE bytes!!!"
    exit 1
fi

pg_dump -h "$DB_HOST" -U "$DB_USER" -F t -f "$BACKUP_FILE" "$DB_BASE" >/dev/null 2>&1
dump_exit_code=$?


if [ $dump_exit_code -eq 0]; then
    log_message "Backup is created!!!"
else
    log_message "Backup is not created!!!"
    exit 1
fi

remove

log_message "End"