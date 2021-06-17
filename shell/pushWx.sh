#!/usr/bin/env bash
set -exo
TokenFile="token.data"
to_user=("openidxx" "openidxx")

function PushWarning()
{
# $1:tousr	
# $2:msg	 
# $3:token
    curl -X POST -d "{\"touser\":\"${1}\",\"msgtype\":\"text\",\"text\":{\"content\":\"${2}\"}}" "https://api.weixin.qq.com/cgi-bin/message/custom/send?access_token=${3}"
}
function GetToken()
{
	now=$(date +%s)
	if [ -f $TokenFile ] && [ $((now-$(date +%s -r $TokenFile))) -lt 1800 ];then
		cat $TokenFile
	else
    		local appid="wxab5c18132c84a29a"
	    	local secret="1a54b8b926c19dc4ff1dfa52ef494abf"
    		local token=$(curl "https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=${appid}&secret=${secret}"|jq ".access_token"|sed 's#\"##g')
		echo $token > $TokenFile
		echo $token
	fi
}

token=$(GetToken)
echo "token=${token}"
msg="aaddcc"
for user in ${to_user[@]}
do
PushWarning ${user} "${msg}" ${token}
done
