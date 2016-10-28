clear
itemIndex=10;
filename=['MotorUnits/Fibers/MU_',num2str(itemIndex),'_Localized.mat'];
load(filename)
jitter
analyseJitter
[del,numFibers,del2]=size(segments);
for fiber=1:numFibers
    figure;
    plot(squeeze(segments(:,fiber,:))')
    cls
end
