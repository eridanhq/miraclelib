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
  run timeout --foreground 30s "$PWD/../client/ecmctl" --getversion
  assert_failure
}

######################################################################################################
############
############
############
############
####################           Client without server should timeout
############
############
############
############
######################################################################################################
@test "ecmctl --sysinit" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --sysinit
  assert_failure
}

@test "ecmctl --getfreq" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --getfreq
  assert_failure
}

@test "ecmctl --getstats" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --getstats
  assert_failure
}

@test "ecmctl --setfreq" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --setfreq
  assert_failure
}

@test "ecmctl --getpwr" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --getpwr
  assert_failure
}

@test "ecmctl --setpwr" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --setpwr
  assert_failure
}

@test "ecmctl --getsamplerate" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --getsamplerate
  assert_failure
}

@test "ecmctl --setsamplerate" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --setsamplerate
  assert_failure
}

@test "ecmctl --getrxfreq" {
  run "$PWD/../client/ecmct" --getrxfreq
  assert_failure
}

@test "ecmctl --setrxfreq" {
  run "$PWD/../client/ecmct" --setrxfreq
  assert_failure
}

@test "ecmctl --getrxsamplerate" {
  run "$PWD/../client/ecmct" --getrxsamplerate
  assert_failure
}

@test "ecmctl --setrxsamplerate" {
  # XXX must start miracle_controller
  run "$PWD/../client/ecmctl" --setrxsamplerate
  assert_failure
}

@test "ecmctl --getrxgains" {
  run "$PWD/../client/ecmctl" --getrxgains
  assert_failure
}

@test "ecmctl --setrxgains" {
  run "$PWD/../client/ecmctl" --setrxgains
  assert_failure
}

@test "ecmctl --sysoff" {
  run "$PWD/../client/ecmctl" --sysoff
  assert_failure
}

@test "ecmctl --startscp" {
  run "$PWD/../client/ecmctl" --startscp
  assert_failure
}

@test "ecmctl --prepscp" {
  run "$PWD/../client/ecmctl" --prepscp
  assert_failure
}

@test "ecmctl --resetnow" {
  run "$PWD/../client/ecmctl" --resetnow
  assert_failure
}

@test "ecmctl --resetdone" {
  run "$PWD/../client/ecmctl" --resetdone
  assert_failure
}

@test "ecmctl --sendupdates" {
  run "$PWD/../client/ecmctl" --sendupdates
  assert_failure
}

@test "ecmctl --checkupdates" {
  run "$PWD/../client/ecmctl" --checkupdates
  assert_failure
}

@test "ecmctl --getversion" {
  skip
  run "$PWD/../client/ecmctl" --getversion
  assert_failure
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

teardown() {
  sudo killall -9 miracle_controller || /bin/true
}

@test "Good ecmctl --sysinit" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --sysinit
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpwr" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpwr
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setpwr" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpwr
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --sysoff" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --sysoff
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --startscp" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --startscp
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --prepscp" {
  skip
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --prepscp
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --resetnow" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --resetnow
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --resetdone" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --resetdone
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --sendupdates" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --sendupdates
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --checkupdates" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --checkupdates
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getversion" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getversion
  [ "$status" -eq 0 ]
}


######################################################################################################
############
############
############
############
####################            Run multiple commands
############
############
############
############
######################################################################################################

@test "Multi: sysinit & sysoff" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --sysinit
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --sysoff
  [ "$status" -eq 0 ]
}

@test "Multi: getver & getstats" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getversion
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getstats
  [ "$status" -eq 0 ]
}

@test "Multi: getfreq & getstats" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getstats
  [ "$status" -eq 0 ]
}

@test "Multi: setfreq & getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getfreq
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: setsamplerate & getsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: setpower & getpower" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: setrxfreq & getrxfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxfreq
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: setrxsamplerate & getrxsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: setrxgains & getrxgains" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: all TX gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower
  [ "$status" -eq 0 ]
}

@test "Multi: all RX gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains
  [ "$status" -eq 0 ]
}

@test "Multi: all gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxfreq
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains
  [ "$status" -eq 0 ]
}

@test "Multi: 10 TX getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getfreq
    [ "$status" -eq 0 ]
  done
}

@test "Multi: 100 TX getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..100}
  do
    run "$PWD/../client/ecmctl" --getfreq
    [ "$status" -eq 0 ]
  done
}

@test "Multi: 10 TX getsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getsamplerate
    [ "$status" -eq 0 ]
  done
}

@test "Multi: 10 TX getpwr" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getpwr
    [ "$status" -eq 0 ]
  done
}

@test "Multi: 10 RX getrxsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxsamplerate
    [ "$status" -eq 0 ]
  done
}

@test "Multi: 10 RX getrxgains" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxgains
    [ "$status" -eq 0 ]
  done
}

@test "Multi: 10 RX getrxfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxfreq
    [ "$status" -eq 0 ]
  done
}



# Question : can we do get commands without sysinit?
# Run multiple commands back-to-back    DONE
# loop through each get command         DONE
# loop through each set command
# loop through multiple commands
# Limits of set commands (min-max-above limits)