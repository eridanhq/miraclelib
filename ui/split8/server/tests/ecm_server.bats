#!/usr/bin/env ../../bats/test/libs/bats/bin/bats
load '../../bats/test/libs/bats-support/load'
load '../../bats/test/libs/bats-assert/load'

@test "bats test" {
  assert_success
}

@test "Server exists" {
  echo $PWD/../server/miracle_controller 
  run  $PWD/../server/miracle_controller --bg
  #[ "$status-eq 0 ]
  [ "$?" -eq 0 ]
  assert_success
}

teardown() {
  sudo /usr/bin/killall -9 miracle_controller || /bin/true
}

@test "Help message from server" {
  run "$PWD/../server/miracle_controller" -h
  #[ "$status" -eq 0 ]
  assert_output --partial "Hello, Welcome to Miracle Controller Server!!!"
}

@test "Server start on port 9400" {
  run "$PWD/../server/miracle_controller" --bg
  assert_success
  socks="$(/usr/bin/netstat -ln|grep 9400|/usr/bin/wc -l)"
  [ "$socks" -ge 1 ]
}

@test "Server process in background" {
  run "$PWD/../server/miracle_controller" --bg
  assert_success
}

@test "Server process running" {
  run "$PWD/../server/miracle_controller" --bg
  assert_success
  numps="$(/usr/bin/ps ax|grep miracle_controller|/usr/bin/wc -l)"
  [ "$numps" -ge 1 ]
}
