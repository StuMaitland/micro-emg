x={};
broken={};
% fiber_working={};
for itemIndex=10:23
    filename=['MotorUnits/Fibers/MU_',num2str(itemIndex),'_Localized.mat'];
    load(filename)
    disp(filename)
    try
        %jitter
        analyseJitter
        %x=[x,MCD];
        input_working={};
        [del,numFibers,del2]=size(segments);
        disp('Number of Fibers: ')
        disp(numFibers)
        for fiber=1:numFibers
            plot(squeeze(segments(:,fiber,:))')
            input_working=[input_working, input('How many fibres: ', 's')];
        end
        fiber_working{itemIndex}={input_working};
        
    catch
        disp('broken')
        broken=[broken,itemIndex];
    end
    
    clearvars -except x broken fiber_working
end