clear
close all

addpath('D:\MultiChannelEMG\MuscleModel\')
addpath('D:\MultiChannelEMG\DecomposeMEMG\Sensitivity')
%colorMs = colormap(hsv);
close
colors = zeros(10,3);
for i=1:10
    %colors(i,:)=colorMs((i-1)*6+1,:);
    colors(i,:)= [rand rand rand];
end
fixedColor=1;
plotMU=0;
recordMovie=0;
needle = buildNeedleModel('nonlinear',48);
needle(1:10,:)=[];
save('D:\MultiChannelEMG\DecomposeMEMG\Sensitivity\activeNeedle.mat','needle');
ex = needle(:,1)';
ey=needle(:,2)';

MUs= [1 3 5 7];

Fs=20000;
CE = 3.17;
w = 0.6;
options.MaxFunEvals=100000;
options.Display='off';
options.MaxITer= 10000;

time = 0.02;

mu=4;


    

load(['D:\MultiChannelEMG\BBCDEMO\MU for cross correlation\broken channels removed\min 5\MU_' num2str(MUs(mu)) '.mat']);

data=squeeze(mean(MU))';

repeats=50;
results=[];
results2=[];
PredNum=[];    
mixing=[];

for rep=1:repeats
    [icasig, A, W] = fastica (data');
    
    orig_icasig = icasig;

    template = zeros(1,size(icasig,2));
    template(200)=1;
    load('SVMClassifierForICA.mat')

    for j=1:size(icasig,1)
        icasig(j,:) = (icasig(j,:)-min(icasig(j,:)))/(max(icasig(j,:))-min(icasig(j,:)));
        [a,b,c]=alignsignals(template,icasig(j,:));
        if c<0
            b(1:abs(c))=icasig(j,1);
        end
        if c>0
            b(1:length(b)-c)= icasig(j,c+1:end);
            b(length(b)-c:end) = icasig(j,end);
        end
        icasig(j,:) = b(1:401);
    end

    pred=SVMClassifierICA.predictFcn(icasig); 



    %%

    idx = find(pred==0);
    PredNum(rep) = length(idx);
    
    mx=[];
    mxAdded=[];
    mxRsquare=[];
    mxResults=[];
    mxAddedResults=[];
    mxAddedRsquare=[];
    
    selectedComps=[];
    
    for i=1:length(idx)
        mxT = abs(A(:,idx(i)));
        mxT = (mxT-min(mxT))/(max(mxT)-min(mxT));
        [Vmax,xp]=max(mxT);
        param=zeros(1,5);
        param(1)=CE;
        param(2)=w;
        param(3)=Vmax;
        param(4)=ex(xp)+rand;
        param(5)=ey(xp)+rand;
        result=fminsearch(@(x) evaluateExpFun(x,mxT),param,options);
        rsq = rsquare(mxT,result);
        disp(rsq);
        if rsq>0.85
            mx(end+1,:)=mxT;
            mxResults(end+1,:) = result;
            mxRsquare(end+1)=rsq;
        else
            overlap = overlapped(mxT');
            if ~isempty(overlap)
                for k=1:size(overlap,1)
                    [Vmax,xp]=max(overlap(k,:));
                    param=zeros(1,5);
                    param(1)=CE;
                    param(2)=w;
                    param(3)=Vmax;
                    param(4)=ex(xp)+rand;
                    param(5)=ey(xp)+rand;
                    result=fminsearch(@(x) evaluateExpFun(x,overlap(k,:)'),param,options);
                    rsq = rsquare(overlap(k,:)',result);
                    if isnan(rsq)
                        disp('STOP');
                    end
                    disp(rsq);
                    if rsq>0.8
                        mxAddedResults(end+1,:)=result;
                        mxAdded(end+1,:) = overlap(k,:);
                        mxAddedRsquare(end+1)=rsq;
                    end
                end
            end
        end

    end
    [resultsRep,mxC,rsquares]=analyseTempResults(mx,mxAdded,mxResults,mxAddedResults,mxRsquare,mxAddedRsquare);
    results=[results;resultsRep];
    mixing =[mixing;mxC];
    results2=[results2;mxResults;mxAddedResults];
end
%%

disp(mean(PredNum));

locations = results(:,[4 5]);
allParams=results;
locationsY = locations(:,2);
rmIdx=find((locationsY<0.185 & locationsY>-0.185) | locationsY> 2 | locationsY <-2);
locations(rmIdx,:)=[];
allParams(rmIdx,:)=[];
mixing(rmIdx,:)=[];
locationsX = locations(:,1);
rmIdx = find(locationsX<min(needle(:,1))-0.2 | locationsX>max(needle(:,1))+0.2);
locations(rmIdx,:)=[];
allParams(rmIdx,:)=[];
mixing(rmIdx,:)=[];

locationsMU{mu}=locations;

[C,ptsC,centers]= dbscan(locations',0.2,5);
scatter(needle(:,1),needle(:,2),'s','fill')
hold on
scatter(locations(:,1),locations(:,2))
scatter(centers(1,:),centers(2,:),'fill')

clusterParams=zeros(max(unique(ptsC)),size(allParams,2));
figure;
for c=1:max(unique(ptsC))
    mxC = mixing(ptsC==c,:);
    clusterParams(c,:)=mean(allParams(ptsC==c,:));
    plot(mean(mxC));
    hold on
end
channels = size(data,2);
time = size(data,1);
nfb = max(unique(ptsC));
dist=zeros(channels,nfb);
sen =zeros(channels,nfb);
for i=1:nfb
    dist(:,i)=sqrt((centers(1,i)-needle(:,1)).^2+(centers(2,i)-needle(:,2)).^2);
    %sen(:,i)=ones(channels,1)./dist(:,i);
    CE= clusterParams(i,1);
    w = clusterParams(i,2);
    a = clusterParams(i,3);
    sen(:,i)=ones(channels,1).*exp(-CE*dist(:,i).^w);
end

trans= inv(sen'*sen)*sen';
Aest=[];
for t=1:size(data,1)
    Aest(:,t)=trans*data(t,:)';
end

V=[];
for i=1:nfb
    %V(i,:,:)=repmat(Aest(i,:)',1,channels)./repmat(dist(:,i)',time,1);
    V(i,:,:)=repmat(Aest(i,:)',1,channels).*repmat(sen(:,i)',time,1);
end

expData = squeeze(sum(V));

figure;
surf(data);
title('Real Data');
figure;
surf(expData);
title('Reconstructed Data');
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