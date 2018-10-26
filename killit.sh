now=$(ps -a | grep "assignment2")
first=${now%% *}
echo $first
kill -9 $first

