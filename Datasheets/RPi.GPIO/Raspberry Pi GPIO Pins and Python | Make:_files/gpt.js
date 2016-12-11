/**
 * jquery visible
 * Copyright 2012, Digital Fusion
 * Licensed under the MIT license.
 * http://teamdf.com/jquery-plugins/license/
 *
 * @author Sam Sehnert
 * @desc A small plugin that checks whether elements are within
 *       the user visible viewport of a web browser.
 *       only accounts for vertical position, not horizontal.
 */
(function(e){e.fn.visible=function(t,n,r){var i=e(this).eq(0),s=i.get(0),o=e(window),u=o.scrollTop(),a=u+o.height(),f=o.scrollLeft(),l=f+o.width(),c=i.offset().top,h=c+i.height(),p=i.offset().left,d=p+i.width(),v=t===true?h:c,m=t===true?c:h,g=t===true?d:p,y=t===true?p:d,b=n===true?s.offsetWidth*s.offsetHeight:true,r=r?r:"both";if(r==="both")return!!b&&m<=a&&v>=u&&y<=l&&g>=f;else if(r==="vertical")return!!b&&m<=a&&v>=u;else if(r==="horizontal")return!!b&&y<=l&&g>=f}})(jQuery);

/**
 * @file gpt.js
 * All javascript for ad creating.
 *
 * @Author Ben Voran
 */

(function (make, window, document, $, undefined) {

  // Set global
  make = window.make = window.make || {};

  // Set object merge function
  /**
   * Function make.extend()
   *
   * @description
   * Merge defaults with user options
   * Could be shorter but supports IE < 9
   */
  make.extend = function (defaults, options) {
    var extended = {};
    var prop;
    for (prop in defaults) {
      if (Object.prototype.hasOwnProperty.call(defaults, prop)) {
        extended[prop] = defaults[prop];
      }
    }
    for (prop in options) {
      if (Object.prototype.hasOwnProperty.call(options, prop)) {
        extended[prop] = options[prop];
      }
    }
    return extended;
  };

  // namespace for advertisement related functions
  make.gpt = {

    /**
     * Function make.gpt.init()
     *
     * @description
     * loads dependencies & set global vars
     *
    **/
    init: function() {

      if (!window.googletag) {

        window.googletag = {cmd:[]};
        (function() { var gads = document.createElement('script'); gads.async = true; gads.type = 'text/javascript'; var useSSL = 'https:' == document.location.protocol; gads.src = (useSSL ? 'https:' : 'http:') + '//www.googletagservices.com/tag/js/gpt.js'; var node = document.getElementsByTagName('script')[0]; node.parentNode.insertBefore(gads, node); })();

        googletag.cmd.push(function() {
          googletag.pubads().collapseEmptyDivs();
          // page level targeting
          if (window.ad_vars && ad_vars.page) {
            googletag.pubads().setTargeting("page", ad_vars.page);
          }
          if (window.ad_vars && ad_vars.cat) {
            googletag.pubads().setTargeting("cat", ad_vars.cat);
          }
          if (window.ad_vars && ad_vars.tags) {
            googletag.pubads().setTargeting("tags", ad_vars.tags);
          }
          // Test
          if (window.location.hostname !== "makezine.com") {
            googletag.pubads().setTargeting('t', ['y']);
          }
          // Ad tracking
          googletag.pubads().addEventListener('slotRenderEnded', function(event) {
            // Use GA if available.
            if (window.ga && !make.gpt.adsTracked) {
              ga('set', 'dimension1', 'true');
              make.gpt.adsTracked = 1;
            }
          });
        });

      }

      $(document).ready(function(){
        // Try loading any placeholder for ads.
        make.gpt.loadDyn();
      });

    },

    /**
     * Array make.gpt.scrollAds
     *
     * @description
     *  array to store ads that load on scroll.
     */
    scrollAds: [],

    /**
     * Function make.gpt.scrollLoad()
     *
     * @description
     *  Setup scroll function.
     */
    scrollLoad: function() {
      if (this.scrollAds && this.scrollAds.length) {
        // Set scroll loading.
        $(document).on("scroll", make.gpt.scrollLoadAd);
      }
    },

    /**
     * Function make.gpt.scrollLoadAd()
     *
     * @description
     *  Scroll function to load ads.
     */
    scrollLoadAd: function() {
      // Prevent build up of multiple scroll functions.
      clearTimeout(gptScrollTimer);
      var gptScrollTimer = setTimeout(function(){
        var scrollAds = make.gpt.scrollAds.slice(0);
        // Loop through scroll load ads.
        for (var i = 0; i < scrollAds.length; i++) {
          // Is ad in viewport.
          if ($(scrollAds[i]).visible(true)) {
            var index = make.gpt.scrollAds.indexOf(scrollAds[i]);
            // Remove from original array.
            make.gpt.scrollAds.splice(index, 1);
            // Load ad.
            make.gpt.load($(scrollAds[i]).filter(':empty'));
          }
        }
        // Stop scroll event if all ads are loaded.
        if (!make.gpt.scrollAds.length) {
          $(document).off("scroll", make.gpt.scrollLoadAd);
        }
      },100);
    },

    /**
     * Function make.gpt.loadDyn()
     *
     * @description
     *  loads placeholder ads if visible.
     */
    loadDyn: function($elem) {
      $elem = !$elem || !$elem.size() ? $('.js-ad') : $elem;
      $elem.filter(':empty').each(function(){
        var $t = $(this);
        // Add to scrollAds.
        if ($t.is('.scroll-load') && window.make_ads_scroll_load) {
          if (make.gpt.scrollAds.indexOf($t) === -1) {
            make.gpt.scrollAds.push($t[0]);
          }
        }
        // Load ad.
        else {
          make.gpt.load($t);
        }
      });
      if (make.gpt.scrollAds.length) {
        // Set scroll load funciton.
        make.gpt.scrollLoad();
        $(document).scroll();
      }
      
    },

    /**
     * Function make.gpt.load()
     *
     * @description
     *  renders ads from placeholders in the DOM. 
     *  example placeholder: <div class='js-ad' data-size='[[300,250]]' data-pos='"btf"'></div>
     */
    load: function($elem) {
      // Loop through js ads.
      $elem.each(function(){
        var $t = $(this),
            a = {};
        // Attempt to parse data attributes.
        a.size = [300,250]; // default.
        try {
          a.size =  $t.attr('data-size') ? JSON.parse($t.attr('data-size')) : null;
          a.sizeMap = $t.attr('data-size-map') ? JSON.parse($t.attr('data-size-map')) : null;
          a.pos = $t.attr('data-pos') ? JSON.parse($t.attr('data-pos')) : null;
          a.adVars = $t.attr('data-ad-vars') ? JSON.parse($t.attr('data-ad-vars')) : null;
          a.viewport = $t.attr('data-viewport') ? JSON.parse($t.attr('data-viewport')) : null;
        }
        catch (e) {
          if (window.console && typeof console.log == 'function') {
            console.log('error parsing ad data-attributes');
          }
        }
        // Generate ad from placeholder vars.
        var ad = make.gpt.getVars(a.size),
            adDiv = '<div id="' + ad.slot + '" class="make_ad ' + a.size.join().replace(/\[\]/g,'').replace(/,/g,'x') + '"></div>';
        $t.append(adDiv);
        // Do with custom ad vars.
        if (a.adVars) {
          make.gpt.setAd({'size' : a.size, 'sizeMap' : a.sizeMap, 'pos' : a.pos, 'adPos' : ad.adPos, 'slot' : ad.slot, 'tile' : ad.tile, 'viewport' : a.viewport, 'page' : a.adVars.page, 'cat' : a.adVars.cat, 'tags' : a.adVars.tags, 'zone' : a.adVars.zone});
        }
        // Do with page defined ad vars.
        else {
          make.gpt.setAd({'size' : a.size, 'sizeMap' : a.sizeMap, 'pos' : a.pos, 'adPos' : ad.adPos, 'slot' : ad.slot, 'tile' : ad.tile, 'viewport' : a.viewport});
        }
      });
    },

    /**
     * Function make.gpt.setAd()
     *
     * @description
     *  creates an ad to display
     */
    setAd: function(options) {

      var defaults = {
            'size': [300,250],
            'pos': 'btf', // @TODO: needs refactoring.
            'adPos': 1,
            'tile': 1,
            'slot': "ad_300x250_1",
            'zone': window.ad_vars ? ad_vars.zone : "/11548178/Makezine/",
            'sizeMap': null,
            'viewport' : null,
            'page': window.ad_vars ? ad_vars.page : null,
            'cat': window.ad_vars ? ad_vars.cat : null,
            'tags': window.ad_vars ? ad_vars.tags : null,
            'companion': false,
            'custom': window.ad_vars && ad_vars.custom_target_name ? [ad_vars.custom_target_name, ad_vars.custom_target_value] : null
          },
          ad = make.extend(defaults, options),
          winWidth = document.documentElement.clientWidth,
          display = ad.viewport == 'small' && winWidth > 767 || ad.viewport == 'large' && winWidth < 768 ? false : true;
      // Call the ad
      if (display) {
        googletag.cmd.push(function() {

          // Define ad
          window[ad.slot] = googletag.defineSlot(ad.zone, ad.size, ad.slot).addService(googletag.pubads());

          // Do companion
          if (ad.companion) {
            window[ad.slot].addService(googletag.companionAds());
          }

          // Set size map
          if (ad.sizeMap) {
            window[ad.slot].defineSizeMapping(ad.sizeMap);
          }

          // Set targeting
          if (ad.page) {
            window[ad.slot].setTargeting("page", ad.page);
          }
          if (ad.cat) {
            window[ad.slot].setTargeting("cat", ad.cat);
          }
          if (ad.tags) {
            window[ad.slot].setTargeting("tags", ad.tags);
          }
          if (ad.custom) {
            window[ad.slot].setTargeting(ad.custom[0], ad.custom[1]);
          }

          // Set pos & tile
          window[ad.slot].setTargeting("pos", [ad.pos]);
          window[ad.slot].setTargeting("adPos", [ad.adPos]);
          window[ad.slot].setTargeting("tile", [ad.tile]);
          document.getElementById(ad.slot).setAttribute("data-adPos", ad.adPos);
          document.getElementById(ad.slot).setAttribute("data-tile", ad.tile);
          document.getElementById(ad.slot).setAttribute("data-tags", JSON.stringify(ad));

          // Display Ad
          googletag.enableServices();
          googletag.display(ad.slot);

        });
      }

    },

    /**
     * Function make.gpt.getVars()
     *
     * @description
     *  creates an ad position specific to the size & title
     */
    getVars: function(adSize) {

      var sizeStr = adSize instanceof Array ? adSize.join("x").replace(/,/g,"x") : undefined ,
          a = sizeStr ? {} : sizeStr;
      if (sizeStr) {
        this["adPos" + sizeStr] = this["adPos" + sizeStr] || 1;
        a.adPos = this["adPos" + sizeStr]++;
        this.tile = this.tile || 1;
        a.tile = this.tile++;
        a.slot = 'ad_' + sizeStr + '_' + a.adPos;
      }
      return a;
    },

    /**
     * Function make.gpt.refresh()
     *
     * @description
     *  refreshes all ads with a given class name.
     */
    refresh: function(className) {

      var $adElems = className ? $(className) : $('.ad-refresh');
      // Loop through each ad element.
      $adElems.each(function(){
        // Trigger refresh.
        var slotname = $(this).find('[id*="ad_"]').attr("id");
        googletag.pubads().refresh([window[slotname]]);
      });
    },

    /**
     * Function make.gpt.injectAds()
     *
     * @description
     *  Injects ads inbetween elements.
     */
    injectAds: function($items, options) {

      var defaults = {
            'markup': '<div class=\'js-ad scroll-load\' data-size=\'[[300,250]]\' data-pos=\'"btf"\'></div>',
            'skipCount': 3,
            'max': -1, // Unlimited
            'renderIntial': true
          },
          ops = make.extend(defaults, options),
          count = 0;

      // Loop through each element.
      $items.each(function(i){
        // Find the desired skipCount and inject.
        if (((i===0 && ops.renderIntial) || ((i+1) % ops.skipCount === 0)) && (count < ops.max || ops.max < 0)) {
          $(this).before(ops.markup);
          // Render injected markup.
          var $ad = $(this).is('.js-ad') ? $(this) : $(this).find('.js-ad');
          make.gpt.loadDyn($ad);
          count++;
        }
      });
    }

  };

  make.gpt.init();

})(this.make, this, this.document, jQuery);
