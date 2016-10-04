%% find peaks

peakTimes = zeros(size(MU,1),nfb);
for d=1:size(MU,1)
    dmu = squeeze(MU(d,:,:));
    for i=1:nfb
        %closest channel
        [~,ch]= max(sen(:,i));
        %closets time point
        [~,ti]= max(abs(Aest(i,:)));
        segment = squeeze(MU(d,ch,ti-20:ti+19));
        st = (ti-10)*50; %in micro seconds
        t = 1:40;
        f = fit(t', segment,  'smoothingspline');
        xt = 0.02:0.02:40;
        [~,shift]=max(abs(feval(f,xt)));
        peakTimes(d,i) = st+shift; %in micro second
    end
end

%% calculate 
tmShift=[];
MCD=zeros(nfb,nfb);
for ref=1:nfb
    for i=1:nfb
        tmShift(:,i)=abs(peakTimes(:,i)-peakTimes(:,ref));
        sShift= diff(tmShift(:,i));
        %disp(mean(abs(sShift)));
        sShift(abs(sShift)>50)=[];
        %disp(100*length(sShift)/size(tmShift,1));

        MCD(ref,i) = mean(abs(sShift));
    end
end

figure;
plot(MCD(2:end),'+--','LineWidth',2)

%%
clear clusterFigure
paintDelay=0;
runGMMClusteirng

%% Jitter Histogram

assignedColor=jiiterHistogram(MCD(2:end));
assignedColor=[[0 0 1];assignedColor];
clear clusterFigure
paintDelay=0;
runGMMWithJitterColorCode