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

@test "ecmctl --getpower" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --getpower
  assert_failure
}

@test "ecmctl --setpower" {
  run timeout --foreground 30s "$PWD/../client/ecmctl" --setpower
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
  skip
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
####################            Test arguments for each command
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

@test "Good ecmctl --getfreq 1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq T1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq TX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq trx1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq TRX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq 2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq T2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq TX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq trx2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getfreq TRX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq TRX2
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --getfreq 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getfreq TX3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq tx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getfreq trx3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq trx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getfreq abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getfreq abcdef
  assert_failure
}

@test "Good ecmctl --getstats 1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats T1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats TX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats trx1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats TRX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats 2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats T2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats TX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats trx2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats TRX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats TRX2
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --getstats 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getstats TX3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats tx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getstats trx3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats trx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getstats abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats abcdef
  assert_failure
}

@test "Good ecmctl --setfreq 1 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq 1 3.2e4
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setfreq T1 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq T1 3.2e4
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setfreq T 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq T 3.2e4
  assert_failure
}

@test "Good ecmctl --setfreq TX1 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq TX1 3.2e4
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setfreq t 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq T 3.2e4
  assert_failure
}

@test "Good ecmctl --setfreq T 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq T 3.2e4
  assert_failure
}

@test "Good ecmctl --setfreq T2 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq T2 3.2e4
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setfreq TX2 3.2e42" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq TX2 3.2e4
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setfreq TRX2 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq TRX2 3.2e4
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setfreq trx2 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq trx2 3.2e4
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setfreq T3 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq T3 3.2e4
  assert_failure
}

@test "Bad ecmctl --setfreq TX3 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq TX3 3.2e4
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setfreq TRX3 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq TRX3 3.2e4
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setfreq t3 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq t3 3.2e4
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setfreq abcdef 3.2e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq abcdef 3.2e4
  [ "$status" -ne 0 ]
}

@test "Good ecmctl --getpower 1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower T1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower TX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower trx1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower TRX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower 2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower T2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower TX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower trx2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower TRX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower TRX2
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --getpower 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getpower TX3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower tx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getpower trx3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower trx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getpower abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower abcdef
  assert_failure
}

@test "Good ecmctl --setpower 1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower 1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setpower T1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower T1 100
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setpower T 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower T 100
  assert_failure
}

@test "Good ecmctl --setpower TX1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower TX1 100
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setpower t 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower T 100
  assert_failure
}

@test "Good ecmctl --setpower T 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower T 100
  assert_failure
}

@test "Good ecmctl --setpower T2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower T2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setpower TX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower TX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setpower TRX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower TRX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setpower trx2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower trx2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setpower T3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower T3 100
  assert_failure
}

@test "Bad ecmctl --setpower TX3 100e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower TX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setpower TRX3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower TRX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setpower t3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower t3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setpower abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower abcdef
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setpower" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower
  assert_failure
}

@test "Bad ecmctl --getsamplerate 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getsamplerate 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getsamplerate TX3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate tx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getsamplerate trx3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate trx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getsamplerate abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate abcdef
  assert_failure
}

@test "Good ecmctl --getsamplerate 1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate T1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate TX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate TX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate trx1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate trx1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate TRX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate 2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate 2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate T2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate T2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate TX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate TX2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate trx2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate trx2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate TRX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate TRX2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate 1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate 1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate T1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate T1 100
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setsamplerate T 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate T 100
  assert_failure
}

@test "Good ecmctl --setsamplerate TX1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate TX1 100
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setsamplerate t 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate T 100
  assert_failure
}

@test "Good ecmctl --setsamplerate T 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate T 100
  assert_failure
}

@test "Good ecmctl --setsamplerate T2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate T2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate TX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate TX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate TRX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate TRX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate trx2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate trx2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate T3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate T3 100
  assert_failure
}

@test "Bad ecmctl --setsamplerate TX3 100e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate TX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setsamplerate TRX3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate TRX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setsamplerate t3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate t3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setsamplerate abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate abcdef
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate
  assert_failure
}

@test "Bad ecmctl --getsamplerate 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxfreq 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxfreq TX3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq tx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxfreq trx3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq trx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --get rxfreq abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq abcdef
  assert_failure
}

@test "Good ecmctl --getrxfreq 1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq T1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq TX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq TX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq trx1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq trx1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq TRX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq 2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq 2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq T2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq T2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq TX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq TX2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq trx2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq trx2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq TRX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq TRX2
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setrxfreq TX3 100e4" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq TX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxfreq TRX3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq TRX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxfreq t3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq t3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxfreq abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq abcdef
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq
  assert_failure
}

@test "Good ecmctl --setrxfreq 1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq 1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxfreq T1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq T1 100
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setrxfreq T 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq T 100
  assert_failure
}

@test "Good ecmctl --setrxfreq TX1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq TX1 100
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setrxfreq t 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq T 100
  assert_failure
}

@test "Good ecmctl --setrxfreq T 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq T 100
  assert_failure
}

@test "Good ecmctl --setrxfreq T2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq T2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxfreq TX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq TX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxfreq TRX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq TRX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxfreq trx2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq trx2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxfreq T3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq T3 100
  assert_failure
}

@test "Bad ecmctl --getrxsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxsamplerate 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxsamplerate TX3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate tx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxsamplerate trx3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrsamplerate  trx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --get rxsamplerate abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate abcdef
  assert_failure
}

@test "Good ecmctl --getrxsamplerate 1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate T1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate TX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate TX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate trx1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate trx1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate TRX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfresamplerate 2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate  2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate T2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate T2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxrxsamplerate X2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate  TX2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate trx2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate trx2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate TRX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX2
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setrxsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate
  assert_failure
}

@test "Bad ecmctl --setrxsamplerate T3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate T3 100
  assert_failure
}

@test "Bad ecmctl --setrxsamplerate TX3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxsamplerate TRX3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TRX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxsamplerate t3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate t3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxsamplerate abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate abcdef
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxsamplerate T 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate  T 100
  assert_failure
}

@test "Good ecmctl --setrxsamplerate 1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate 1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate T1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate T1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate TX1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TX1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate TRX1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TRX1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate trx1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate trx1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate 2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate 2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate T2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate T2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate TX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate TRX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TRX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate trx2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate trx2 100
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --getrxgains" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxgains 3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains 3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxgains TX3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains tx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxxgains trx3" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains  trx3
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --getrxgains abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains abcdef
  assert_failure
}

@test "Good ecmctl --getrxgains 1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains 1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains T1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains T1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains TX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains TX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains trx1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains trx1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains TRX1" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains  TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains 2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains  2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsagains T2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains T2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxrxgains X2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains  TX2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains trx2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains trx2
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains TRX2" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains TRX2
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setrxgains" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains
  assert_failure
}

@test "Bad ecmctl --setrxgains T3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains T3 100
  assert_failure
}

@test "Bad ecmctl --setrxgains TX3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains TX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxgains TRX3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains TRX3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxgains t3 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains t3 100
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxgains abcdef" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains abcdef
  [ "$status" -ne 0 ]
}

@test "Bad ecmctl --setrxgains T 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains T 100
  assert_failure
}

@test "Good ecmctl --setrxgains 1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains 1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains T1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains T1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains TX1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains TX1 100
  [ "$status" -eq 0 ]
}

@test "Bad ecmctl --setrxgains TRX1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setxgains  TRX1 100
  assert_failure
}

@test "Good ecmctl --setrxgains trx1 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains trx1 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains 2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains 2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains T2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains T2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains TX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains TX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains TRX2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains TRX2 100
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains trx2 100" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains trx2 100
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
  run "$PWD/../client/ecmctl" --getfreq TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getstats" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getstats TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setfreq TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getpower" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getpower TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setpower" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setpower TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getsamplerate TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setsamplerate TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxfreq TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxfreq" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxfreq TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxsamplerate TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxsamplerate" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxsamplerate TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --getrxgains" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --getrxgains TRX1
  [ "$status" -eq 0 ]
}

@test "Good ecmctl --setrxgains" {
  $PWD/../server/miracle_controller --bg
  run "$PWD/../client/ecmctl" --setrxgains TRX1
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


# Question : can we do get commands without sysinit?
# Run multiple commands back-to-back    DONE
# loop through each get command         DONE
# loop through each set command
# loop through multiple commands
# Try all cli options for each command
# Limits of set commands (min-max-above limits)