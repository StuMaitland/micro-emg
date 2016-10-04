function [assignedColor]=jiiterHistogram(MCD)
    
    bins = floor(max(MCD)/5)+1;
    %allocate MCD in bins
    binLoc = floor(MCD/5);
    mymap=[0 1 1;...
           1 1 0;...
           0 1 0; ...
           1 0 0];
    maxy0=0;
    usedBins=0;
    assignedColor=zeros(length(MCD),3);
    for i=1:bins
        idx=find(binLoc==i);
        if ~isempty(idx)
            y0=0.5;
            usedBins = usedBins+1;
            for j=1:length(idx)
                rectangle('Position',[i+0.25 y0 0.5 1],'Curvature',1,'FaceColor',mymap(usedBins,:))
                y0=y0+1.2;
                assignedColor(idx(j),:)=mymap(usedBins,:);
            end
            if maxy0<y0
                maxy0=  y0;
            end
        end
    end
    bins = bins+4;
    axis([0 bins 0 maxy0])
    ax = gca;
    set(ax,'Color',[1 1 1])
    ax.XTick = 0:bins;
    lbls =cell(1,bins);
    for i=0:bins
        lbls{i+1} = num2str(i*5);
    end
    ax.XTickLabel=lbls;
    set(ax,'FontSize',14);
end