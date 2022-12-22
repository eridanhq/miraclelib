#!/usr/bin/env ../../bats/test/libs/bats/bin/bats
load '../../bats/test/libs/bats-support/load'
load '../../bats/test/libs/bats-assert/load'

@test "bats test" {
  assert_success
}

@test "client exists" {
  echo $PWD/../client/ecmctl
  run $PWD/../client/ecmctl
  [ "$status" -eq 0 ]
}

@test "server exists" {
  echo $PWD/../server/miracle_controller
  run $PWD/../server/miracle_controller
  [ "$status" -eq 0 ]
}

@test "ecmctl -h" {
  run "$PWD/../client/ecmctl" -h
  # [ "$status" -eq 0 ]
  assert_output --partial "Ecmctl take the following options:"
}

@test "ecmctl --help" {
  run "$PWD/../client/ecmctl" -help
  # [ "$status" -eq 0 ]
  assert_output --partial "Ecmctl take the following options:"
}

@test "ecmctl -x" {
  run "$PWD/../client/ecmctl" -x
  # [ "$status" -eq 0 ]
  assert_output --partial "Ecmctl take the following options:"
}

@test "ecmctl --blah" {
  run "$PWD/../client/ecmctl" --blah
  # [ "$status" -eq 0 ]
  assert_output --partial "Ecmctl take the following options:"
}

@test "ecmctl -getversion" {
  run "$PWD/../client/ecmctl" -getversion
  assert_failure
}

@test "ecmctl --getversion" {
  # XXX must start miracle_controller
  nohup $PWD/../server/miracle_controller &
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getversion
  [ "$status" -eq 0 ]
}

@test "ecmctl --sysinit" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --sysinit
  [ "$status" -eq 0 ]
}

@test "ecmctl --getfreq" {
   # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getfreq
  [ "$status" -eq 0 ]
}

@test "ecmctl --getstats" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getstats
  [ "$status" -eq 0 ]
}

@test "ecmctl --setfreq" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --setfreq
  [ "$status" -eq 0 ]
}

@test "ecmctl --getpwr" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getpwr
  [ "$status" -eq 0 ]
}

@test "ecmctl --setpwr" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --setpwr
  [ "$status" -eq 0 ]
}

@test "ecmctl --getsamplerate" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getsamplerate
  [ "$status" -eq 0 ]
}

@test "ecmctl --setsamplerate" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --setsamplerate
  [ "$status" -eq 0 ]
}

@test "ecmctl --getrxfreq" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --getrxfreq
  [ "$status" -eq 0 ]
}

@test "ecmctl --setrxfreq" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --setrxfreq
  [ "$status" -eq 0 ]
}

@test "ecmctl --getrxsamplerate" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --getrxsamplerate
  [ "$status" -eq 0 ]
}

@test "ecmctl --setrxsamplerate" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --setrxsamplerate
  [ "$status" -eq 0 ]
}

@test "ecmctl --getrxgains" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --getrxgains
  [ "$status" -eq 0 ]
}

@test "ecmctl --setrxgains" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --setrxgains
  [ "$status" -eq 0 ]
}

@test "ecmctl --sysoff" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --sysoff
  [ "$status" -eq 0 ]
}

@test "ecmctl --startscp" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --startscp
  [ "$status" -eq 0 ]
}

@test "ecmctl --prepscp" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --prepscp
  [ "$status" -eq 0 ]
}

@test "ecmctl --resetnow" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --resetnow
  [ "$status" -eq 0 ]
}

@test "ecmctl --resetdone" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --resetdone
  [ "$status" -eq 0 ]
}

@test "ecmctl --sendupdates" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --sendupdates
  [ "$status" -eq 0 ]
}

@test "ecmctl --checkupdates" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --checkupdates
  [ "$status" -eq 0 ]
}

@test "ecmctl --getversion" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getversion
  [ "$status" -eq 0 ]
}

######################################################################################################
############
############
############
############
####################            Start server and run all commands
############
############
############
############
######################################################################################################

@test "Good ecmctl --sysinit" {
  # must start miracle_controller
  nohup $PWD/../server/miracle_controller &
  run timeout --foreground 10s "$PWD/../client/ecmctl" --sysinit
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getfreq
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getstats
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setfreq" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --setfreq
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpwr" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getpwr
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setpwr" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --setpwr
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --getsamplerate
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate" {
  # XXX must start miracle_controller
  run timeout --foreground 10s "$PWD/../client/ecmctl" --setsamplerate
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --getrxfreq
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxfreq" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --setrxfreq
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --getrxsamplerate
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --setrxsamplerate
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --getrxgains
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --setrxgains
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --sysoff" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --sysoff
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --startscp" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --startscp
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --prepscp" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --prepscp
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --resetnow" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --resetnow
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --resetdone" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --resetdone
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --sendupdates" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --sendupdates
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --checkupdates" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --checkupdates
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getversion" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --getversion
  [ "$status" -eq 0 ]
}
# Run multiple commands back-to-back
# loop through each command
# loop through multiple commands