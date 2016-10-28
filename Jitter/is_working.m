% This converts the count of working fibers from the raw input from the
% user, to an array of integers, and calculates the total number.

load 'jitter_MU.mat'
for unit=1:23
    result = cellfun(@str2num, fiber_working{unit}, 'UniformOutput', false);
    % No of working fibers
    count{1,unit}=sum(cell2mat(result));
    % No of total fibers detected
    count{2,unit}=length(fiber_working{unit});
    % Which fibers are working
    count{unit}=cell2mat(result);
    for item in result
end

