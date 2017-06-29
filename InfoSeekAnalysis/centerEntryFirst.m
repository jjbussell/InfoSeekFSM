for f = 1:a.numFiles
   firstCenterEntryTxns = [];
   centerEntries = [];
   firstCenterEntry = [];
   firstCenterEntryTxns = a.firstCenterEntryTxn(a.fileAll == f);
   centerEntries = a.centerEntries(a.centerEntries(:,1)==f,:);
   for t = 1:numel(firstCenterEntryTxns)
      centerEntryFirstDiff = firstCenterEntryTxns(t,1) - centerEntries(:,2); 
      centerEntryFirstDiff(centerEntryFirstDiff < 0) = inf;
     [centerEntryFirstVal, centerEntryFirstIdx] = min(centerEntryFirstDiff);
     if ~isinf(centerEntryFirstVal)
         firstCenterEntry(t,:) = centerEntries(centerEntryFirstIdx,:);
     else
         firstCenterEntry(t,:) = [f 0 t 0 0 0 0 0];
     end
   end
   if f == 1
       a.firstCenterEntry = firstCenterEntry;       
   else
       a.firstCenterEntry = [a.firstCenterEntry; firstCenterEntry];
   end
end

