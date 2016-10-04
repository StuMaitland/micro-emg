peakTimes = zeros(size(MU,1),nfb);
segments  = zeros(size(MU,1),nfb,2000);
for d=1:size(MU,1)
    dmu = squeeze(MU(d,:,:));
    for i=1:nfb
        %closest channel
        [~,ch]= max(sen(:,i));
        %closets time point
        [~,ti]= max(abs(Aest(i,:)));
        segment = squeeze(MU(d,ch,ti-20:ti+19));
        t = 1:40;
        f = fit(t', segment,  'smoothingspline');
        xt = 0.02:0.02:40;
        segments(d,i,:)= feval(f,xt);
        [~,shift]=max(abs(feval(f,xt)));
    end
end
