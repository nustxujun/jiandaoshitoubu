function getCurrentLevel(exp)
	lvl = 1
	while exp >= getNeedExp(lvl) do
		lvl = lvl + 1
	end
	
	return lvl
end

function getNeedExp(level)
	return math.pow(2,level) + 4;
end