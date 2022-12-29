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

teardown() {
  sudo killall -9 miracle_controller || /bin/true
}

@test "Multi: sysinit & sysoff" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --sysinit
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --sysoff
  [ "$status" -eq 0 ]
}

@test "Multi: (1) getver & getstats" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getversion
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getstats TRX1
  [ "$status" -eq 0 ]
}

@test "Multi: (2) getver & getstats" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getversion
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getstats TRX2
  [ "$status" -eq 0 ]
}

@test "Multi: (1) getfreq & getstats" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getstats TRX1
  [ "$status" -eq 0 ]
}

@test "Multi: (2) getfreq & getstats" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getstats TRX2
  [ "$status" -eq 0 ]
}

@test "Multi: (1) setfreq & getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getfreq TRX1
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (2) setfreq & getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getfreq TRX2
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (1) setsamplerate & getsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate TRX1
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (2) setsamplerate & getsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate TRX2
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (1) setpower & getpower" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower TRX1
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (2) setpower & getpower" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower TRX2
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (1) setrxfreq & getrxfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxfreq TRX1
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (2) setrxfreq & getrxfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxfreq TRX2
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (1) setrxsamplerate & getrxsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX1
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (2) setrxsamplerate & getrxsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX2
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (1) setrxgains & getrxgains" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains TRX1
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (2) setrxgains & getrxgains" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains TRX2
  [ "$status" -eq 0 ]
  # TODO : make sure gets what was set
}

@test "Multi: (1) all TX gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower TRX1
  [ "$status" -eq 0 ]
}

@test "Multi: (2) all TX gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower TRX2
  [ "$status" -eq 0 ]
}

@test "Multi: (1) all RX gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains TRX1
  [ "$status" -eq 0 ]
}

@test "Multi: (2) all RX gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains TRX2
  [ "$status" -eq 0 ]
}

@test "Multi: (1) all gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxfreq TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX1
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains TRX1
  [ "$status" -eq 0 ]
}

@test "Multi: (2) all gets" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getsamplerate TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getpower TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxfreq TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX2
  [ "$status" -eq 0 ]
  run "$PWD/../client/ecmctl" --getrxgains TRX2
  [ "$status" -eq 0 ]
}

@test "Multi: (1) 10 TX getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getfreq TRX1
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (2) 10 TX getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getfreq TRX2
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (1) 100 TX getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..100}
  do
    run "$PWD/../client/ecmctl" --getfreq TRX1
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (2) 100 TX getfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..100}
  do
    run "$PWD/../client/ecmctl" --getfreq TRX2
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (1) 10 TX getsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getsamplerate TRX1
    [ "$status" -eq 0 ]
  done
}
@test "Multi: (2) 10 TX getsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getsamplerate TRX2
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (1) 10 TX getpower" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getpower TRX1
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (2) 10 TX getpower" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getpower TRX2
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (1) 10 RX getrxsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxsamplerate TRX1
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (2) 10 RX getrxsamplerate" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxsamplerate TRX2
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (1) 10 RX getrxgains" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxgains TRX1
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (2) 10 RX getrxgains" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxgains TRX2
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (1) 10 RX getrxfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxfreq TRX1
    [ "$status" -eq 0 ]
  done
}

@test "Multi: (2) 10 RX getrxfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10}
  do
    run "$PWD/../client/ecmctl" --getrxfreq TRX2
    [ "$status" -eq 0 ]
  done
}

@test "Multi: 1000 RX getrxfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..1000}
  do
    run "$PWD/../client/ecmctl" --getrxfreq TRX1
    [ "$status" -eq 0 ]
    run "$PWD/../client/ecmctl" --getrxfreq TRX2
    [ "$status" -eq 0 ]
  done
}

@test "Multi: 10000 RX getrxfreq" {
  # must start miracle_controller
  $PWD/../server/miracle_controller --bg
  for i in {1..10000}
  do
    run "$PWD/../client/ecmctl" --getrxfreq TRX1
    [ "$status" -eq 0 ]
    run "$PWD/../client/ecmctl" --getrxfreq TRX2
    [ "$status" -eq 0 ]
  done
}


# Question : can we do get commands without sysinit?
# Run multiple commands back-to-back    DONE
# loop through each get command         DONE
# loop through each set command
# loop through multiple commands
# Try all cli options for each command
# Limits of set commands (min-max-above limits)